#ifdef TEST_CONSOLE

#include <Logger.h>
#include <LuaRepl.h>

#include "../DllInterface.h"

#include "game_messages.h"
#include "lua_events.h"
#include "nco.h"

static bool ExecuteLuaFile(const char* filePath)
{
	auto& executeResult = NcoLuaRuntime().ExecuteFile(filePath);
	auto isError = executeResult.IsErrorResult();

	if (isError)
	{
		LogError("%s script error: %s", filePath, executeResult.GetError());
	}

	delete &executeResult;

	return !isError;
}

static void TestLuaRules() {
	LogInfo("Testing Lua rules");

	if (!("test-lua-rules.lua"))
	{
		LogError("Lua rules test script failed");
	}
}

static void GenerateApiDocs() {
	LogInfo("Generating Lua API docs");

	if (!ExecuteLuaFile("generate-lua-api-docs.lua"))
	{
		LogError("Lua API docs generation script failed");
	}
}

static void DumpRules() {
	LogInfo("Dumping all rules");

	if (!ExecuteLuaFile("dump-rules.lua"))
	{
		LogError("Rule dumping script failed");
	}
}

static void TestSpecialRules() {
	LogInfo("Testing special rules");

	auto special = SpecialClass();

	special.Init();
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

static void TestLuaEvents()
{
	LogDebug("Test Console: Testing Lua Events");

	LogInfo("Testing setting rules from scenario start event handler");

	OnScenarioLoad("SCG01EA");

	LogInfo("Testing setting rules from save load event handler");

	OnSaveLoad(NcoTypeConverter().ToString(HOUSE_GOOD).GetValue(), 4);

	LogInfo("Testing game tick event handler");

	TestGameLoopMessage(GAME_TICK_ELAPSED);
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

static void Pause()
{
	#ifndef CI_ENV
	system("pause");
	#endif
}

static void Parse_Command_Line(const char* commandLine)
{
	if (StringStartsWith(commandLine, "--lua-repl"))
	{
		#ifdef CI_ENV
		puts("ERROR: CI environment detected - Lua REPL not supported");

		exit(1);
		#else
		auto& luaRepl = LuaRepl::Build(NcoLuaRuntime());

		luaRepl.Enter();

		delete &luaRepl;

		exit(0);
		#endif
	}
	else if (StringStartsWith(commandLine, "--dump-rules"))
	{
		if (!ExecuteLuaFile("dump-rules.lua"))
		{
			puts("ERROR: Failed to dump rules file");
			exit(1);
		}

		exit(0);
	}
	else if (StringStartsWith(commandLine, "--generate-api-docs"))
	{
		if (!ExecuteLuaFile("generate-lua-api-docs.lua"))
		{
			puts("ERROR: Failed to generate api docs");
			exit(1);
		}

		exit(0);
	}
}

/// <summary>
/// Test console app for the New Construction Options mod.
/// </summary>
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int windowState)
{
	StartConsoleOutput();

	puts("========================");
	puts("  NCO Mod: Test Console  ");
	puts("=========================");

	if (!NcoStartup())
	{
		NcoShutdown();

		puts("Test Console: ERROR! NcoStartup failed - bailing out!");
		Pause();

		return 1;
	}

	puts(NcoTypeConverter().ToString(HOUSE_BAD).GetValue());

	Parse_Command_Line(commandLine);

	TestSpecialRules();

	TestLuaRules();

	DumpRules();

	GenerateApiDocs();

	// mock DLL interface event handler
	Add_Event_Callback_Proxy((CNC_Event_Callback_Type)&Game_Event_Callback);

	TestLuaEvents();

	NcoShutdown();

	puts("Test Console: finishing normally");

	Pause();

	StopConsoleOutput();

	return 0;
}

#endif
