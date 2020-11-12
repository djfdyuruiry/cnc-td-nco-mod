#include "function.h"

#include "lua_repl.h"
#include "TiberianDawnNcoRuntime.h"

static HANDLE LUA_REPL_THREAD;
static HANDLE LUA_EVENT_THREAD;

static DWORD WINAPI Start_Lua_Repl(LPVOID lpParam)
{
	Enter_Lua_Repl(false);

	return 0;
}

bool NCO_Startup()
{
	NcoRulesRuntime().EnsureRulesIniIsLoaded();

	Log_Info("New Construction Options mod starting up");

	if (!TdNcoRuntime().RulesInitWasSuccessful())
	{
		Show_Error("Aborting game launch because rules INI failed validation.\n\nPlease check your rules are valid.");
		return false;
	}

	if (NcoRulesRuntime().LuaIsEnabled())
	{
		if (!TdNcoRuntime().LuaInitWasSuccessful())
		{
			Show_Error("Aborting game launch as their were errors initialising Lua");
			return false;
		}
	}
	else
	{
		Log_Warn("Lua is not enabled in rules file - scripts will be ignored and NOT run");
	}

	Log_Info("New Construction Options mod has started successfully");

	#ifndef TEST_CONSOLE
	if (NcoRulesRuntime().LuaConsoleIsEnabled()) {
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

		Log_Info("Lua console opened");
	}
	#endif

	if (NcoRulesRuntime().LuaIsEnabled())
	{
		Log_Info("Attempting to start Lua Event thread");

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

		Log_Info("Lua Event thread started");
	}

	return true;
}

void NCO_Shutdown()
{
	Log_Info("New Construction Options mod shutting down");

	auto& runtime = TdNcoRuntime();

	#ifndef TEST_CONSOLE
	if (NcoRulesRuntime().LuaConsoleIsEnabled())
	{
		if (LUA_REPL_THREAD != NULL && LUA_REPL_THREAD != INVALID_HANDLE_VALUE && !TerminateThread(LUA_REPL_THREAD, 0))
		{
			Log_Error("Error closing Lua console: %s", Get_Win32_Error_Message());
		}

		Stop_Console_Output();

		Log_Info("Lua Console shut down");
	}
	#endif

	if (NcoRulesRuntime().LuaIsEnabled())
	{
		if (LUA_EVENT_THREAD != NULL && LUA_EVENT_THREAD != INVALID_HANDLE_VALUE && !TerminateThread(LUA_EVENT_THREAD, 0))
		{
			Log_Error("Error stopping Lua event thread: %s", Get_Win32_Error_Message());
		}
	}

	TiberianDawnNcoRuntime::Shutdown();

	// this must be the last call - otherwise the file might be reopened by a log call
	Close_Log_File_If_Open();
}
