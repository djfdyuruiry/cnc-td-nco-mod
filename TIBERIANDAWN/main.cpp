#ifdef TEST_CONSOLE

#include "function.h"
#include "lua_repl.h"

static void Test_Lua_Events()
{
	Log_Info("Testing setting rules from scenario start event handler");

	On_Scenario_Load("SCG01EA");

	Log_Info("Testing returning early from scenario start event handler");

	On_Scenario_Load("SCG04EA");
}

static void Test_Special_Rules() {
	Log_Info("Testing special rules");

	auto special = SpecialClass();

	special.Init();

	Log_Info("Testing cached special rules ");

	special.Init();
}

static void Pause()
{
	#ifndef CI_ENV
	system("pause");
	#endif
}

static void Configure_Console_Output()
{
	#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console allocation");
	#else
	if (!AttachConsole(ATTACH_PARENT_PROCESS) 
		&& !AllocConsole()
		&& !AttachConsole(GetCurrentProcessId()))
	{
		Log_Error("Error opening Win32 console: %s", Get_Win32_Error_Message());

		exit(1);
	}

	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	freopen("CON", "w", stdin);
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
	Configure_Console_Output();

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

	Test_Special_Rules();

	Log_Debug("Test Console: Testing Lua Events");

	Test_Lua_Events();

	NCO_Shutdown();

	puts("Test Console: finishing normally");

	Pause();

	return 0;
}

#endif
