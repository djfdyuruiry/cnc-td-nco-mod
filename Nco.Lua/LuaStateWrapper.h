#pragma once

#include "ILuaStateWrapper.h"

class LuaStateWrapper : public ILuaStateWrapper
{
private:
	lua_State* lua;

	LuaStateWrapper(lua_State* lua) : lua(lua)
	{
	}

	LuaResult& BuildResult(bool errorOccurredDuringExecution)
	{
		return LuaResult::Build(
			errorOccurredDuringExecution ? GetLastError() : NULL
		);
	}

public:
	static ILuaStateWrapper& Build(lua_State* lua)
	{
		return *(new LuaStateWrapper(lua));
	}

	LuaResult& ExecuteScript(const char* script)
	{
		return BuildResult(
			luaL_dostring(lua, script)
		);
	}

	LuaResult& ExecuteFile(const char* filePath)
	{
		return BuildResult(
			luaL_dofile(lua, filePath)
		);
	}

	LuaResultWithValue<int>& ReadInteger(int stackIndex)
	{
		if (!lua_isinteger(lua, stackIndex))
		{
			return LuaResultWithValue<int>::BuildWithError(0, "Value is not an integer");
		}

		return LuaResultWithValue<int>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	LuaResultWithValue<double>& ReadDouble(int stackIndex)
	{
		if (!lua_isnumber(lua, stackIndex))
		{
			return LuaResultWithValue<double>::BuildWithError(0.0, "Value is not a double");
		}

		return LuaResultWithValue<double>::BuildWithValue(
			luaL_checknumber(lua, stackIndex)
		);
	}

	LuaResultWithValue<bool>& ReadBool(int stackIndex)
	{
		if (!lua_isboolean(lua, stackIndex))
		{
			return LuaResultWithValue<bool>::BuildWithError(false, "Value is not a boolean");
		}

		return LuaResultWithValue<bool>::BuildWithValue(
			lua_toboolean(lua, stackIndex)
		);
	}

	LuaResultWithValue<const char*> ReadString(int stackIndex)
	{
		if (!lua_isstring(lua, stackIndex))
		{
			return LuaResultWithValue<const char*>::BuildWithError(NULL, "Value is not a string");
		}

		return LuaResultWithValue<const char*>::BuildWithValue(
			luaL_checkstring(lua, stackIndex)
		);
	}
	LuaResultWithValue<int>& ReadInteger()
	{
		return ReadInteger(lua_gettop(lua));
	}

	LuaResultWithValue<double>& ReadDouble()
	{
		return ReadDouble(lua_gettop(lua));
	}

	LuaResultWithValue<bool>& ReadBool()
	{
		return ReadBool(lua_gettop(lua));
	}

	LuaResultWithValue<const char*> ReadString()
	{
		return ReadString(lua_gettop(lua));
	}

	const char* GetLastError()
	{
		return lua_tostring(lua, -1);
	}

	void WriteInteger(int value)
	{
		lua_pushinteger(lua, value);
	}

	void WriteDouble(double value)
	{
		lua_pushnumber(lua, value);
	}

	void WriteBool(bool value)
	{
		lua_pushboolean(lua, value);
	}

	void WriteString(const char* value)
	{
		lua_pushstring(lua, value);
	}

	void WriteFunction(const char* name, lua_CFunction function)
	{
		lua_pushcfunction(lua, function);
		lua_setglobal(lua, name);
	}
};
