#ifdef TEST_CONSOLE

#include "function.h"
#
#include "DllInterface.h"

#include "lua_repl.h"
#include "rules_cache.h"

static void Test_Lua_Rules() {
	Log_Info("Testing Lua rules");

	if (!Execute_Lua_File("test-lua-rules.lua"))
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

static void Test_Lua_Events()
{
	Log_Debug("Test Console: Testing Lua Events");

	Log_Info("Testing setting rules from scenario start event handler");

	On_Scenario_Load("SCG01EA");

	Log_Info("Testing setting rules from save load event handler");

	On_Save_Load(HOUSE_GOOD, 4);

	Log_Info("Testing game tick event handler");

	Push_Game_Loop_Message(GAME_TICK_ELAPSED);
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

static void Run_Special_Test_If_Present()
{
	if (true)
	{
		return;
	}

	while (true)
	{
		auto gameRuleValue = Read_Int_Game_Rule("MaxHarvesterCapacity", 28, INT_MIN, INT_MAX);
		
		gameRuleValue = Read_Int_Game_Rule("MaxHarvesterCapacity", 28, INT_MIN, INT_MAX);
		gameRuleValue = Read_Int_Game_Rule("MaxHarvesterCapacity", 28, INT_MIN, INT_MAX);
		gameRuleValue = Read_Int_Game_Rule("MaxHarvesterCapacity", 28, INT_MIN, INT_MAX);

		Sleep(25);

		printf(".");
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
		Enter_Lua_Repl() ? exit(0) : exit (1);
		#endif
	}
	else if (String_Starts_With(commandLine, "--dump-rules"))
	{
		if (!Initialise_Lua() || !Execute_Lua_File("dump-rules.lua"))
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

	Parse_Command_Line(commandLine);

	if (!NCO_Startup())
	{
		NCO_Shutdown();

		puts("Test Console: ERROR! NCO_Startup failed - bailing out!");
		Pause();

		return 1;
	}

	Run_Special_Test_If_Present();

	Add_Event_Callback_Proxy((CNC_Event_Callback_Type)&Game_Event_Callback);

	Test_Special_Rules();

	Test_Lua_Events();

	Test_Lua_Rules();

	Dump_Rules();

	NCO_Shutdown();

	puts("Test Console: finishing normally");

	Pause();

	Stop_Console_Output();

	return 0;
}

#endif
