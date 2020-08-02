#include "function.h"

#include "lua.h"
#include "lua_repl.h"

static HANDLE LUA_REPL_THREAD;
static HANDLE LUA_EVENT_THREAD;

static DWORD WINAPI Start_Lua_Repl(LPVOID lpParam)
{
	Enter_Lua_Repl(false);

	return 0;
}

bool NCO_Startup()
{
	puts("New Construction Options mod starting up");

	Ensure_Rules_Ini_Is_Loaded();

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

		Start_Console_Output();

		LUA_REPL_THREAD = CreateThread(
			NULL,
			0,
			&Start_Lua_Repl,
			NULL,
			NULL,
			NULL
		);

		if (LUA_REPL_THREAD == NULL || LUA_REPL_THREAD == INVALID_HANDLE_VALUE)
		{
			Show_Error("Failed to start Lua console: %s", Get_Win32_Error_Message());
		}
	}
	#endif

	if (Lua_Is_Enabled())
	{
		LUA_EVENT_THREAD = CreateThread(
			NULL,
			0,
			&Process_Game_Loop_Messages,
			NULL,
			NULL,
			NULL
		);

		if (LUA_EVENT_THREAD == NULL || LUA_EVENT_THREAD == INVALID_HANDLE_VALUE)
		{
			Show_Error("Failed to start Lua event thread: %s", Get_Win32_Error_Message());
			return false;
		}
	}

	return true;
}

void NCO_Shutdown()
{
	Log_Info("New Construction Options mod shutting down");

	#ifndef TEST_CONSOLE
	if (Lua_Console_Is_Enabled())
	{
		if (!TerminateThread(LUA_REPL_THREAD, 0))
		{
			Show_Error("Error closing Lua console: %s", Get_Win32_Error_Message());
		}

		Stop_Console_Output();
	}
	#endif

	if (Lua_Is_Enabled())
	{
		if (LUA_EVENT_THREAD != NULL && !TerminateThread(LUA_EVENT_THREAD, 0))
		{
			Show_Error("Error stopping Lua event thread: %s", Get_Win32_Error_Message());
		}

		Shutdown_Lua();
	}

	Close_Log_File_If_Open();

	Free_Rules_Memory();
}
