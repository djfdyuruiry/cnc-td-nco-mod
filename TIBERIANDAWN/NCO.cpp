#include "function.h"

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

	return true;
}

void NCO_Shutdown()
{
	Log_Info("New Construction Options mod shutting down");

	// ensure log file is closed before we lose the reference to the handle
	Close_Log_File_If_Open();
}
