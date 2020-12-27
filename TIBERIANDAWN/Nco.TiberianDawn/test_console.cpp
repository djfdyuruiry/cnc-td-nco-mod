#ifdef TEST_CONSOLE

#include <Logger.h>

#include "../DllInterface.h"

#include "game_messages.h"
#include "lua_events.h"
#include "nco.h"

static void ExecuteLuaFile(const char* filePath)
{
	auto& executeResult = NcoLuaRuntime().ExecuteFile(filePath);

	if (executeResult.IsErrorResult())
	{
		ShowErrorAndExit("%s script error: %s", filePath, executeResult.GetError());
	}

	delete &executeResult;
}

static void TestLuaRules() {
	LogInfo("Testing Lua rules");

	ExecuteLuaFile("test-lua-rules.lua");
}

static void GenerateApiDocs() {
	LogInfo("Generating Lua API docs");

	ExecuteLuaFile("generate-lua-api-docs.lua");
}

static void DumpRules() {
	LogInfo("Dumping all rules");

	ExecuteLuaFile("dump-rules.lua");
}

static void TestGameLoopMessage(GameLoopMessageType message)
{
	PushGameLoopMessage(message);

	while (GameLoopMessagesPending())
	{
		Sleep(10);
	}

	ProcessGameUiMessages();
}

static void Game_Event_Callback(const EventCallbackStruct& event)
{
	if (event.EventType == CALLBACK_EVENT_MESSAGE)
	{
		LogInfo(
			"Game message emitted for %f seconds: %s",
			event.Message.TimeoutSeconds,
			event.Message.Message
		);
	}
}

static void TestLuaEvents()
{
	LogDebug("Test Console: Testing Lua Events");

	// mock DLL interface event handler
	Add_Event_Callback_Proxy((CNC_Event_Callback_Type)&Game_Event_Callback);

	LogInfo("Testing setting rules from scenario start event handler");

	OnScenarioLoad("SCG01EA");

	LogInfo("Testing setting rules from save load event handler");

	OnSaveLoad(NcoTypeConverter().ToString(HOUSE_GOOD).GetValue(), 4);

	LogInfo("Testing game tick event handler");

	TestGameLoopMessage(GAME_TICK_ELAPSED);
}

static void Pause()
{
	#ifndef CI_ENV
	system("pause");
	#endif
}

static void RunTests()
{
	puts("========================");
	puts("  NCO Mod: Test Console  ");
	puts("=========================");

	TestLuaRules();

	DumpRules();
	GenerateApiDocs();

	TestLuaEvents();

	NcoShutdown();

	puts("Test Console: finishing normally");
	Pause();
}

static int Parse_Command_Line(const char* commandLine)
{
	if (StringStartsWith(commandLine, "--lua-repl"))
	{
		#ifdef CI_ENV
		ShowErrorAndExit("ERROR: CI environment detected - Lua REPL not supported");
		#else

		auto& replResult = NcoLuaRuntime().ExecuteScript("Nco.LuaRepl.enter()");

		if (replResult.IsErrorResult())
		{
			ShowErrorAndExit("Lua REPL Error: %s", replResult.GetError());
		}

		#endif
	}
	else if (StringStartsWith(commandLine, "--dump-rules"))
	{
		DumpRules();
	}
	else if (StringStartsWith(commandLine, "--generate-api-docs"))
	{
		ExecuteLuaFile("generate-lua-api-docs.lua");
	}
	else
	{
		RunTests();
	}

	return 0;
}

/// <summary>
/// Test console app for the New Construction Options mod.
/// </summary>
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int windowState)
{
	if (!NcoStartup())
	{
		ShowErrorAndExit("Test Console: ERROR! NcoStartup failed - bailing out!");
	}

	return Parse_Command_Line(commandLine);
}

#endif
