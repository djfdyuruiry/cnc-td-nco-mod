#pragma once

#include <functional>
#include <vector>

#include <Utils.h>

#include "ILuaStateWrapper.h"
#include "LuaType.h"

class LuaStateWrapper : public ILuaStateWrapper
{
private:
	lua_State* lua;
	bool stateOwner;

	LuaStateWrapper(lua_State* lua, bool stateOwner) : lua(lua), stateOwner(stateOwner)
	{
		LuaType::InitIfRequired();
	}

	LuaResult& BuildResult(bool errorOccurredDuringExecution)
	{
		return LuaResult::Build(
			errorOccurredDuringExecution ? GetLastError() : NULL
		);
	}

	bool IsType(int typeCode, int stackIndex)
	{
		return lua_type(lua, stackIndex) == typeCode;
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
	static ILuaStateWrapper& Build(lua_State* lua, bool stateOwner = true)
	{
		return *(new LuaStateWrapper(lua, stateOwner));
	}

	~LuaStateWrapper()
	{
		if (stateOwner && lua != NULL)
		{
			lua_close(lua);
		}
	}

	int GetStackTop()
	{
		return lua_gettop(lua);
	}

	int GetUpvalueStackIndex(int upvalueIndex)
	{
		return lua_upvalueindex(upvalueIndex);
	}

	const char* GetLastError()
	{
		return ToString(-1);
	}

	const char* ToString(int stackIndex)
	{
		if (IsBool(stackIndex))
		{
			return ConvertBooleanToString(
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

	const LuaType& GetLuaType(int stackIndex)
	{
		return LuaType::Parse(
			GetType(stackIndex)
		);
	}

	const LuaType& GetLuaType()
	{
		return GetLuaType(GetStackTop());
	}

	bool IsString(int stackIndex)
	{
		return IsType(LUA_TSTRING, stackIndex);
	}

	bool IsTable(int stackIndex)
	{
		return IsType(LUA_TTABLE, stackIndex);
	}

	bool IsTable()
	{
		return IsTable(GetStackTop());
	}

	bool IsBool(int stackIndex)
	{
		return IsType(LUA_TBOOLEAN, stackIndex);
	}

	bool IsInt(int stackIndex)
	{
		return IsNumber(stackIndex) && lua_isinteger(lua, stackIndex);
	}

	bool IsNumber(int stackIndex)
	{
		return IsType(LUA_TNUMBER, stackIndex);
	}

	bool IsNil(int stackIndex)
	{
		return lua_isnil(lua, stackIndex);
	}

	bool IsString()
	{
		return IsString(GetStackTop());
	}

	bool IsBool()
	{
		return IsBool(GetStackTop());
	}

	bool IsInt()
	{
		return IsInt(GetStackTop());
	}

	bool IsNumber()
	{
		return IsNumber(GetStackTop());
	}

	bool IsNil()
	{
		return IsNil(GetStackTop());
	}

	int GetTableSize(int stackIndex)
	{
		return lua_rawlen(lua, stackIndex);
	}

	void IterateOverTable(int stackIndex, std::function<void()> iterateAction)
	{
		lua_pushvalue(lua, stackIndex);
		lua_pushnil(lua);

		auto valIndex = -2;

		while (lua_next(lua, valIndex))
		{
			lua_pushvalue(lua, valIndex);

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

	LuaResultWithValue<long long>& ReadBigInteger(int stackIndex)
	{
		if (!lua_isinteger(lua, stackIndex))
		{
			return LuaResultWithValue<long long>::BuildWithError("Value is not an integer");
		}

		return LuaResultWithValue<long long>::BuildWithValue(
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

	LuaResultWithValue<void*>& ReadUserData(int stackIndex)
	{
		if (!lua_isuserdata(lua, stackIndex))
		{
			return LuaResultWithValue<void*>::BuildWithError("Value is not userdata");
		}

		return LuaResultWithValue<void*>::BuildWithValue(
			lua_touserdata(lua, stackIndex)
		);
	}

	LuaResultWithValue<int>& ReadInteger()
	{
		return ReadInteger(lua_gettop(lua));
	}

	LuaResultWithValue<long long>& ReadBigInteger()
	{
		return ReadBigInteger(lua_gettop(lua));
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

	LuaResultWithValue<void*>& ReadUserData()
	{
		return ReadUserData(GetStackTop());
	}

	void WriteInteger(int value)
	{
		lua_pushinteger(lua, value);
	}

	void WriteBigInteger(long long value)
	{
		lua_pushinteger(lua, value);
	}

	void WriteNumber(double value)
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

	void WriteMethod(const char* name, void* objectPtr, lua_CFunction methodProxy)
	{
		lua_pushlightuserdata(lua, objectPtr);

		lua_pushcclosure(lua, methodProxy, 1);

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
