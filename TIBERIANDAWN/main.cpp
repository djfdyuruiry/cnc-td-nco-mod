#ifdef TEST_CONSOLE

#include "function.h"

static void Test_Lua_Events()
{
	Log_Info("Testing setting rules from scenario start event handler");
	On_Scenario_Start(4);

	Log_Info("Testing returning early from scenario start event handler");
	On_Scenario_Start(1);
}

static void Configure_Console_Output()
{
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
	}

	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
}

/// <summary>
/// Test console app for the New Construction Options mod.
/// </summary>
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	puts("========================");
	puts("  NCO Mod: Test Console  ");
	puts("=========================");

	Configure_Console_Output();

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

	return 0;
}

#endif
