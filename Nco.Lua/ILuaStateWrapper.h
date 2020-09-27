#pragma once

#include <lua.hpp>
#include "LuaResult.h"
#include "LuaResultWithValue.h"

class ILuaStateWrapper
{
public:
	virtual LuaResult& ExecuteScript(const char* script) = 0;

	virtual LuaResult& ExecuteFile(const char* filePath) = 0;

	virtual LuaResultWithValue<int>& ReadInteger(int stackIndex) = 0;

	virtual LuaResultWithValue<double>& ReadDouble(int stackIndex) = 0;

	virtual LuaResultWithValue<bool>& ReadBool(int stackIndex) = 0;

	virtual LuaResultWithValue<const char*> ReadString(int stackIndex) = 0;

	virtual const char* GetLastError() = 0;

	virtual void WriteInteger(int value) = 0;

	virtual void WriteDouble(double value) = 0;

	virtual void WriteBool(bool value) = 0;

	virtual void WriteString(const char* value) = 0;
};
