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
	void PushOntoStack(int stackIndex)
	{
		lua_pushvalue(lua, stackIndex);
	}

	void PopFromStack(int stackIndex)
	{
		lua_pop(lua, stackIndex);
	}

	void SetIndex(int tableIndex, int index)
	{
		lua_rawseti(lua, tableIndex, index);
	}

	void SetIndex(int tableIndex, const char* index)
	{
		lua_setfield(lua, tableIndex, index);
	}

	bool PopKeyValuePair(int tableIndex)
	{
		return lua_next(lua, tableIndex) != 0;
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

	Result& IterateOverTable(int stackIndex, std::function<void(int, int, const LuaType&)> iterateAction)
	{
		if (!IsTable(stackIndex))
		{
			return Result::BuildWithError("Value at stack index '%d' was not a table", stackIndex);
		}

		lua_pushvalue(lua, stackIndex);

		WriteNil();

		auto keyIndex = -2;
		auto valIndex = -1;

		while (PopKeyValuePair(-2))
		{
			auto valType = GetLuaType(valIndex);

			iterateAction(keyIndex, valIndex, valType);

			PopFromStack(1);
		}

		PopFromStack(1);

		return Result::Build();
	}

	Result& IterateOverTable(std::function<void(int, int, const LuaType&)> iterateAction)
	{
		return IterateOverTable(GetStackTop(), iterateAction);
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

	Result& PushTableFieldOntoStack(int stackIndex, const char* field)
	{
		if (StringIsEmpty(field))
		{
			return Result::BuildWithError("Field passed to ILuaStateWrapper::ReadTableField was null or empty");
		}

		if (!IsTable(stackIndex))
		{
			return Result::BuildWithError("Attempted to read field '%s' from a non-table value", field);
		}

		lua_getfield(lua, stackIndex, field);

		return Result::Build();
	}

	Result& PushTableFieldOntoStack(const char* field)
	{
		return PushTableFieldOntoStack(GetStackTop(), field);
	}

	ResultWithValue<int>& ReadInteger(int stackIndex)
	{
		if (!IsInt(stackIndex))
		{
			return ResultWithValue<int>::BuildWithError("Value is not an integer");
		}

		return ResultWithValue<int>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	ResultWithValue<long long>& ReadBigInteger(int stackIndex)
	{
		if (!IsInt(stackIndex))
		{
			return ResultWithValue<long long>::BuildWithError("Value is not an integer");
		}

		return ResultWithValue<long long>::BuildWithValue(
			luaL_checkinteger(lua, stackIndex)
		);
	}

	ResultWithValue<double>& ReadDouble(int stackIndex)
	{
		if (!IsNumber(stackIndex))
		{
			return ResultWithValue<double>::BuildWithError("Value is not a double");
		}

		return ResultWithValue<double>::BuildWithValue(
			luaL_checknumber(lua, stackIndex)
		);
	}

	ResultWithValue<bool>& ReadBool(int stackIndex)
	{
		if (!IsBool(stackIndex))
		{
			return ResultWithValue<bool>::BuildWithError("Value is not a boolean");
		}

		return ResultWithValue<bool>::BuildWithValue(
			lua_toboolean(lua, stackIndex)
		);
	}

	ResultWithValue<const char*>& ReadString(int stackIndex)
	{
		if (!IsString(stackIndex))
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
