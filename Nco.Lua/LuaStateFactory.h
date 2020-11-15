#pragma once

#include <windows.h>

#include <logger.h>
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

		if (String_Is_Empty(libraryPath))
		{
			Log_Error("Blank library path passed to LuaStateFactory::Build - Lua state will likely be unusable");

			return lua;
		}

		// update package path to include library path
		auto packageScript = FormatString("package.path = [[%s\\lib\\?.lua;]] .. package.path", libraryPath);
		
		Log_Debug("Setting lua package path: %s", packageScript);

		luaL_dostring(lua, packageScript);

		delete packageScript;

		return lua;
	}

};
