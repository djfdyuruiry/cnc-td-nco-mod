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
	DWORD Run()
	{
		#ifndef TEST_CONSOLE
		lua.ExecuteScript("Nco.LuaRepl.enter()");
		#else
		// dummy repl thread for test console
		while (true)
		{
			Sleep(10);
		}
		#endif

		return 0;
	}

public:
	static LuaReplThread& Build(ILuaStateWrapper& lua)
	{
		return *(new LuaReplThread(lua));
	}

	virtual bool Stop()
	{
		if (threadRunning)
		{
			auto& exitResult = lua.ExecuteScript("Nco.LuaRepl.exit()");

			if (exitResult.IsErrorResult())
			{
				LogError("Failed to exit Lua REPL: %s", exitResult.GetError());
			}

			delete &exitResult;
		}

		return Thread::Stop();
	}

};
