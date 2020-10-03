#pragma once

#include <map>
#include <type_traits>
#include <vector>

#include <lua.hpp>

#include "LuaResult.h"
#include "LuaResultWithValue.h"

class ILuaStateWrapper
{
private:
	template<class T> void PushValue(T value)
	{
		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			WriteString(value);
		}
		else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long>)
		{
			WriteInteger(value);
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
		{
			WriteDouble(value);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			WriteBool(value);
		}
		else
		{
			WriteNil();
		}
	}

protected:
	virtual void SetIndex(int tableIndex, int index) = 0;
	virtual void SetIndex(int tableIndex, const char* index) = 0;

public:
	virtual LuaResultWithValue<int>& ReadInteger(int stackIndex) = 0;
	virtual LuaResultWithValue<double>& ReadDouble(int stackIndex) = 0;
	virtual LuaResultWithValue<bool>& ReadBool(int stackIndex) = 0;
	virtual LuaResultWithValue<const char*> ReadString(int stackIndex) = 0;

	virtual LuaResultWithValue<int>& ReadInteger() = 0;
	virtual LuaResultWithValue<double>& ReadDouble() = 0;
	virtual LuaResultWithValue<bool>& ReadBool() = 0;
	virtual LuaResultWithValue<const char*> ReadString() = 0;

	virtual const char* GetLastError() = 0;

	virtual void WriteInteger(int value) = 0;
	virtual void WriteDouble(double value) = 0;
	virtual void WriteBool(bool value) = 0;
	virtual void WriteString(const char* value) = 0;
	virtual void WriteFunction(const char* name, lua_CFunction function) = 0;
	virtual void WriteNil() = 0;
	virtual void WriteTable(unsigned int expectedSize = 0) = 0;

	template<class T> void WriteTable(const std::vector<T>& values)
	{
		WriteTable(values.size());

		auto idx = 1;

		for (auto v : values)
		{
			PushValue<T>(v);

			SetIndex(-2, idx);

			idx++;
		}
	}

	template<class T> void WriteTable(const std::map<char*, T>& values)
	{
		lua_createtable(lua, 0, values.size());

		for (auto const& [k, v] : values)
		{
			PushValue<T>(v);

			SetIndex(-2, k);
		}
	}

	virtual LuaResult& ExecuteScript(const char* script) = 0;
	virtual LuaResult& ExecuteFile(const char* filePath) = 0;
};
