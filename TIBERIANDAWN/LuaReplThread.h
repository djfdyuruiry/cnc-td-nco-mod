#pragma once

#include <Thread.h>

#include "lua_repl.h"

class LuaReplThread : public Thread
{
private:
	LuaReplThread() : Thread("Lua REPL")
	{
	}

protected:
	DWORD Run()
	{
		Enter_Lua_Repl();

		return 0;
	}

public:
	static LuaReplThread& Build()
	{
		return *(new LuaReplThread());
	}

};
