#include "function.h"

static void Test_Lua_Events()
{
	if (!Initialise_Lua())
	{
		printf("Error occurred during Lua initialisation");
		exit(1);
	}

	On_Scenario_Start(10);
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
	Configure_Console_Output();

	if (Rules_Ini_Failed_Validation())
	{
		printf("Rules INI file valiation failed");
		exit(1);
	}

	Test_Lua_Events();

	exit(0);
}
