#pragma once

#include <windows.h>

#include <Logger.h>
#include <lua.hpp>
#include <strings.h>

class LuaStateFactory final
{
private:
	LuaStateFactory()
	{
	}

public:
	static lua_State* Build(const char* libraryPath)
	{
		auto lua = luaL_newstate();

		luaL_openlibs(lua);

		if (StringIsEmpty(libraryPath))
		{
			LogError("Blank library path passed to LuaStateFactory::Build - Lua state will likely be unusable");

			return lua;
		}

		// update package path to include library path
		auto packageScript = FormatString("package.path = [[%s\\lib\\?.lua;]] .. package.path", libraryPath);
		
		LogDebug("Setting lua package path: %s", packageScript);

		luaL_dostring(lua, packageScript);

		delete packageScript;

		return lua;
	}

};
