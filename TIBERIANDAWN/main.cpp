#ifdef TEST_CONSOLE

#include "function.h"

#include "DllInterface.h"

#include "lua_repl.h"

static bool Execute_Lua_File(const char* filePath)
{
	auto& executeResult = NcoLuaRuntime().ExecuteFile(filePath);
	auto isError = executeResult.IsErrorResult();

	if (isError)
	{
		Log_Error("%s script error: %s", filePath, executeResult.GetError());
	}

	delete &executeResult;

	return !isError;
}

static void Test_Lua_Rules() {
	Log_Info("Testing Lua rules");

	if (!("test-lua-rules.lua"))
	{
		Log_Error("Lua rules test script failed");
	}
}

static void Dump_Rules() {
	Log_Info("Testing dumping all rules");

	if (!Execute_Lua_File("dump-rules.lua"))
	{
		Log_Error("Rule dumping script failed");
	}
}

static void Test_Special_Rules() {
	Log_Info("Testing special rules");

	auto special = SpecialClass();

	special.Init();
}

static void Test_Game_Loop_Message(GameLoopMessageType message)
{
	Push_Game_Loop_Message(message);

	while (Game_Loop_Messages_Pending())
	{
		Sleep(10);
	}

	Process_Game_Ui_Messages();
}

static void Test_Lua_Events()
{
	Log_Debug("Test Console: Testing Lua Events");

	Log_Info("Testing setting rules from scenario start event handler");

	On_Scenario_Load("SCG01EA");

	Log_Info("Testing setting rules from save load event handler");

	On_Save_Load(House_Type_To_String(HOUSE_GOOD), 4);

	Log_Info("Testing game tick event handler");

	Test_Game_Loop_Message(GAME_TICK_ELAPSED);
}

static void Game_Event_Callback(const EventCallbackStruct& event)
{
	if (event.EventType == CALLBACK_EVENT_MESSAGE)
	{
		Log_Info(
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
	if (String_Starts_With(commandLine, "--lua-repl"))
	{
		#ifdef CI_ENV
		puts("ERROR: CI environment detected - Lua REPL not supported");

		exit(1);
		#else
		Enter_Lua_Repl();

		exit(0);
		#endif
	}
	else if (String_Starts_With(commandLine, "--dump-rules"))
	{
		if (!Execute_Lua_File("dump-rules.lua"))
		{
			puts("ERROR: Failed to dump rules file");
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
	Start_Console_Output();

	puts("========================");
	puts("  NCO Mod: Test Console  ");
	puts("=========================");

	if (!NCO_Startup())
	{
		NCO_Shutdown();

		puts("Test Console: ERROR! NCO_Startup failed - bailing out!");
		Pause();

		return 1;
	}

	Parse_Command_Line(commandLine);

	Test_Special_Rules();

	Test_Lua_Rules();

	Dump_Rules();

	// mock DLL interface event handler
	Add_Event_Callback_Proxy((CNC_Event_Callback_Type)&Game_Event_Callback);

	Test_Lua_Events();

	NCO_Shutdown();

	puts("Test Console: finishing normally");

	Pause();

	Stop_Console_Output();

	return 0;
}

#endif
