#include "function.h"

#include "lua_repl.h"

static HANDLE LUA_REPL_THREAD;

static DWORD WINAPI Start_Lua_Repl_In_Background(LPVOID lpParam)
{
	Enter_Lua_Repl();

	return 0;
}

bool NCO_Startup()
{
	puts("New Construction Options mod starting up");

	if (Rules_Ini_Failed_Validation())
	{
		Log_Error("Aborting game launch as their validation errors in the rules INI");
		return false;
	}

	if (Lua_Is_Enabled())
	{
		if (!Initialise_Lua())
		{
			Log_Error("Aborting game launch as their were errors initialising Lua");
			return false;
		}
	}
	else
	{
		Log_Warn("Lua is not enabled in rules file - scripts will be ignored and NOT run");
	}

	Log_Info("New Construction Options mod has started successfully");

	#ifndef TEST_CONSOLE
	if (Lua_Console_Is_Enabled()) {
		Log_Info("Attempting to display Lua console");

		Configure_Console_Output();

		CreateThread(
			NULL,
			0,
			&Start_Lua_Repl_In_Background,
			NULL,
			NULL,
			NULL
		);
	}
	#endif

	return true;
}

void NCO_Shutdown()
{
	Log_Info("New Construction Options mod shutting down");

	Close_Log_File_If_Open();

	#ifndef TEST_CONSOLE
	if (Lua_Console_Is_Enabled() && !TerminateThread(LUA_REPL_THREAD, 0))
	{
		Log_Warn("Error closing Lua console: %s", Get_Win32_Error_Message());
	}
	#endif
}
