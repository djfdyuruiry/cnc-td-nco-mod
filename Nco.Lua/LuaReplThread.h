#pragma once

#include <Thread.h>

#include "ILuaStateWrapper.h"

class LuaReplThread : public Thread
{
private:
	ILuaStateWrapper& lua;

	LuaReplThread(ILuaStateWrapper& lua) : Thread("Lua REPL"), lua(lua)
	{
	}

protected:
	// TODO: fix crash on mission restart
	DWORD Run()
	{
		auto& luaReplResult = lua.ExecuteScript("Nco.LuaRepl.enter()");

		if (luaReplResult.IsErrorResult())
		{
			ShowError("Lua REPL exited with an error: %sr", luaReplResult.GetError());

			delete &luaReplResult;

			return 1;
		}

		delete& luaReplResult;

		return 0;
	}

public:
	static LuaReplThread& Build(ILuaStateWrapper& lua)
	{
		return *(new LuaReplThread(lua));
	}

};
