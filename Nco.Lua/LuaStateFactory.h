#pragma once

#include <windows.h>

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

		// update package path to include library path
		auto packageScript = FormatString("package.path = [[%s\\lib\\?.lua;]] .. package.path", libraryPath);
		
		luaL_dostring(lua, packageScript);

		delete packageScript;

		return lua;
	}

};
