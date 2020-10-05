#pragma once

#include <functional>
#include <vector>

#include <Utils.h>

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

protected:
	void SetIndex(int tableIndex, int index)
	{
		lua_rawseti(lua, tableIndex, index);
	}

	void SetIndex(int tableIndex, const char* index)
	{
		lua_setfield(lua, tableIndex, index);
	}

public:
	static ILuaStateWrapper& Build(lua_State* lua)
	{
		return *(new LuaStateWrapper(lua));
	}

	~LuaStateWrapper()
	{
		if (lua != NULL)
		{
			lua_close(lua);
		}
	}

	int GetStackTop()
	{
		return lua_gettop(lua);
	}

	const char* GetLastError()
	{
		return ToString(-1);
	}

	const char* ToString(int stackIndex)
	{
		if (IsBool(stackIndex))
		{
			return Convert_Boolean_To_String(
				lua_toboolean(lua, stackIndex)
			);
		}

		return lua_tostring(lua, stackIndex);
	}

	const char* ToString()
	{
		return ToString(GetStackTop());
	}

	const char* GetType(int stackIndex)
	{
		return luaL_typename(lua, stackIndex);
	}

	const char* GetType()
	{
		return luaL_typename(lua, GetStackTop());
	}

	bool IsTable(int stackIndex)
	{
		return lua_istable(lua, stackIndex) == 1;
	}

	bool IsTable()
	{
		return IsTable(GetStackTop());
	}

	bool IsBool(int stackIndex)
	{
		return lua_isboolean(lua, stackIndex) == 1;
	}

	bool IsNumber(int stackIndex)
	{
		return lua_isnumber(lua, stackIndex);
	}

	bool IsInt(int stackIndex)
	{
		return lua_isinteger(lua, stackIndex);
	}

	bool IsNil(int stackIndex)
	{
		return lua_isnil(lua, stackIndex);
	}

	bool IsBool()
	{
		return IsBool(GetStackTop());
	}

	bool IsNumber()
	{
		return lua_isnumber(lua, GetStackTop());
	}

	bool IsInt()
	{
		return IsInt(GetStackTop());
	}

	bool IsNil()
	{
		return IsNil(GetStackTop());
	}

	int GetTableSize(int stackIndex)
	{
		return lua_rawlen(lua, stackIndex);
	}

	void IterateOverTable(int stackIndex, std::function<void(void)> iterateAction)
	{
		lua_pushvalue(lua, stackIndex);
		lua_pushnil(lua);

		while (lua_next(lua, -2))
		{
			lua_pushvalue(lua, -2);

			if (iterateAction != NULL)
			{
				iterateAction();
			}

			lua_pop(lua, 2);
		}

		lua_pop(lua, 1);
	}

	LuaResultWithValue<int>& ReadInteger(int stackIndex)
	{
		if (!lua_isinteger(lua, stackIndex))
		{
			return LuaResultWithValue<int>::BuildWithError("Value is not an integer");
		}

		return LuaResultWithValue<int>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	LuaResultWithValue<double>& ReadDouble(int stackIndex)
	{
		if (!lua_isnumber(lua, stackIndex))
		{
			return LuaResultWithValue<double>::BuildWithError("Value is not a double");
		}

		return LuaResultWithValue<double>::BuildWithValue(
			luaL_checknumber(lua, stackIndex)
		);
	}

	LuaResultWithValue<bool>& ReadBool(int stackIndex)
	{
		if (!lua_isboolean(lua, stackIndex))
		{
			return LuaResultWithValue<bool>::BuildWithError("Value is not a boolean");
		}

		return LuaResultWithValue<bool>::BuildWithValue(
			lua_toboolean(lua, stackIndex)
		);
	}

	LuaResultWithValue<const char*>& ReadString(int stackIndex)
	{
		if (!lua_isstring(lua, stackIndex))
		{
			return LuaResultWithValue<const char*>::BuildWithError("Value is not a string");
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

	LuaResultWithValue<const char*>& ReadString()
	{
		return ReadString(lua_gettop(lua));
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

	void WriteNil()
	{
		lua_pushnil(lua);
	}

	void WriteTable(unsigned int size)
	{
		lua_createtable(lua, 0, size);
	}

	void ClearStack()
	{
		lua_settop(lua, 0);
	}

	void RaiseError(const char* messageFormat, ...)
	{
		va_list formatArgs;
		va_start(formatArgs, messageFormat);

		auto message = FormatString(messageFormat, formatArgs);

		luaL_error(lua, message);

		va_end(formatArgs);

		delete message;
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
};
