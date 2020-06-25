#ifdef TEST_CONSOLE

#include "function.h"
#include "lua_repl.h"

static bool IS_RUNNING_IN_CI_ENV = false;

static void Test_Lua_Events()
{
	Log_Info("Testing setting rules from scenario start event handler");
	On_Scenario_Start(4);

	Log_Info("Testing returning early from scenario start event handler");
	On_Scenario_Start(1);
}

static void Configure_Console_Output()
{
	if (IS_RUNNING_IN_CI_ENV)
	{
		puts("WARNING: CI environment detected - skipped windows console allocation");
		return;
	}

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
}

static void Parse_Command_Line(const char* commandLine)
{
	if (!String_Starts_With(commandLine, "--lua-repl"))
	{
		return;
	}

	if (IS_RUNNING_IN_CI_ENV)
	{
		puts("ERROR: CI environment detected - Lua REPL not supported");

		exit(1);
	}

	Enter_Lua_Repl();

	exit(0);
}

/// <summary>
/// Test console app for the New Construction Options mod.
/// </summary>
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int windowState)
{
	IS_RUNNING_IN_CI_ENV = Parse_Boolean(
		Convert_String_To_Upper_Case(
			Get_Env_Var_Or_Default("CI", "false")
		)
	);

	Configure_Console_Output();

	puts("========================");
	puts("  NCO Mod: Test Console  ");
	puts("=========================");

	Parse_Command_Line(commandLine);

	if (!NCO_Startup())
	{
		NCO_Shutdown();

		puts("Test Console: ERROR! NCO_Startup failed - bailing out!");

		return 1;
	}

	Log_Debug("Test Console: Testing Lua Events");

	Test_Lua_Events();

	NCO_Shutdown();

	puts("Test Console: finishing normally");

	if (!IS_RUNNING_IN_CI_ENV)
	{
		system("pause");
	}

	return 0;
}

#endif
