#include "function.h"

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
		printf("Rules INI file failed valiation steps");
		exit(1);
	}

	Initialise_Lua();

	On_Scenario_Start(10);

	exit(0);
}
