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

	Result& BuildResult(bool errorOccurredDuringExecution)
	{
		return errorOccurredDuringExecution 
			? Result::BuildWithError(GetLastError())
			: Result::Build();
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

	Result& PushGlobalOntoStack(const char* variable)
	{
		if (StringIsEmpty(variable))
		{
			return Result::BuildWithError("Variable name passed to LuaStateWrapper::PushGlobalOntoStack was null or blank");
		}

		lua_getglobal(lua, variable);

		return Result::Build();
	}

	ResultWithValue<int>& ReadInteger(int stackIndex)
	{
		if (!lua_isinteger(lua, stackIndex))
		{
			return ResultWithValue<int>::BuildWithError("Value is not an integer");
		}

		return ResultWithValue<int>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	ResultWithValue<long long>& ReadBigInteger(int stackIndex)
	{
		if (!lua_isinteger(lua, stackIndex))
		{
			return ResultWithValue<long long>::BuildWithError("Value is not an integer");
		}

		return ResultWithValue<long long>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	ResultWithValue<double>& ReadDouble(int stackIndex)
	{
		if (!lua_isnumber(lua, stackIndex))
		{
			return ResultWithValue<double>::BuildWithError("Value is not a double");
		}

		return ResultWithValue<double>::BuildWithValue(
			luaL_checknumber(lua, stackIndex)
		);
	}

	ResultWithValue<bool>& ReadBool(int stackIndex)
	{
		if (!lua_isboolean(lua, stackIndex))
		{
			return ResultWithValue<bool>::BuildWithError("Value is not a boolean");
		}

		return ResultWithValue<bool>::BuildWithValue(
			lua_toboolean(lua, stackIndex)
		);
	}

	ResultWithValue<const char*>& ReadString(int stackIndex)
	{
		if (!lua_isstring(lua, stackIndex))
		{
			return ResultWithValue<const char*>::BuildWithError("Value is not a string");
		}

		return ResultWithValue<const char*>::BuildWithValue(
			luaL_checkstring(lua, stackIndex)
		);
	}

	ResultWithValue<void*>& ReadUserData(int stackIndex)
	{
		if (!lua_isuserdata(lua, stackIndex))
		{
			return ResultWithValue<void*>::BuildWithError("Value is not userdata");
		}

		return ResultWithValue<void*>::BuildWithValue(
			lua_touserdata(lua, stackIndex)
		);
	}

	ResultWithValue<int>& ReadInteger()
	{
		return ReadInteger(lua_gettop(lua));
	}

	ResultWithValue<long long>& ReadBigInteger()
	{
		return ReadBigInteger(lua_gettop(lua));
	}

	ResultWithValue<double>& ReadDouble()
	{
		return ReadDouble(lua_gettop(lua));
	}

	ResultWithValue<bool>& ReadBool()
	{
		return ReadBool(lua_gettop(lua));
	}

	ResultWithValue<const char*>& ReadString()
	{
		return ReadString(lua_gettop(lua));
	}

	ResultWithValue<void*>& ReadUserData()
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

	void WriteFunction(lua_CFunction function, void* functionInfo)
	{
		lua_pushlightuserdata(lua, functionInfo);

		lua_pushcclosure(lua, function, 1);
	}

	void WriteMethod(void* objectPtr, lua_CFunction methodProxy, void* functionInfo)
	{
		lua_pushlightuserdata(lua, functionInfo);
		lua_pushlightuserdata(lua, objectPtr);

		lua_pushcclosure(lua, methodProxy, 2);
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

	void RaiseError(Result& result)
	{
		RaiseError(result.GetError());
	}

	Result& ExecuteScript(const char* script)
	{
		return BuildResult(
			luaL_dostring(lua, script)
		);
	}

	Result& ExecuteFile(const char* filePath)
	{
		return BuildResult(
			luaL_dofile(lua, filePath)
		);
	}

};
