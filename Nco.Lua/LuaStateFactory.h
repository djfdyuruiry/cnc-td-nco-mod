#pragma once

#include <windows.h>

#include <Logger.h>
#include <lua.hpp>
#include <strings.h>

class LuaStateFactory final
{
private:
	static void InitLuaPacakgePath(const char* libraryPath, lua_State* lua)
	{
		auto packageScript = FormatString("package.path = [[%s\\lib\\?.lua;]] .. package.path", libraryPath);

		LogDebug("Setting lua package path: %s", packageScript);

		luaL_dostring(lua, packageScript);

		delete packageScript;
	}

	static void InitNcoTable(lua_State* lua)
	{
		lua_newtable(lua);

		lua_setglobal(lua, "Nco");
	}

	LuaStateFactory()
	{
	}

public:
	static lua_State* Build(const char* libraryPath)
	{
		auto lua = luaL_newstate();

		luaL_openlibs(lua);

		InitNcoTable(lua);

		if (StringIsEmpty(libraryPath))
		{
			LogError("Blank library path passed to LuaStateFactory::Build - Lua state will likely be unusable");

			return lua;
		}

		InitLuaPacakgePath(libraryPath, lua);

		return lua;
	}

};
