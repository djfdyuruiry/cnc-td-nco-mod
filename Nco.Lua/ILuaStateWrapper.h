#pragma once

#include <functional>
#include <map>
#include <type_traits>
#include <vector>

#include "LuaResult.h"
#include "LuaResultWithValue.h"

class ILuaStateWrapper
{
protected:
	virtual void SetIndex(int tableIndex, int index) = 0;
	virtual void SetIndex(int tableIndex, const char* index) = 0;

public:
	virtual int GetStackTop() = 0;
	virtual const char* GetLastError() = 0;

	virtual const char* ToString(int stackIndex) = 0;
	virtual const char* ToString() = 0;

	virtual const char* GetType(int stackIndex) = 0;
	virtual const char* GetType() = 0;

	virtual bool IsTable(int stackIndex) = 0;
	virtual bool IsBool(int stackIndex) = 0;
	virtual bool IsNumber(int stackIndex) = 0;
	virtual bool IsInt(int stackIndex) = 0;
	virtual bool IsNil(int stackIndex) = 0;

	virtual bool IsTable() = 0;
	virtual bool IsBool() = 0;
	virtual bool IsNumber() = 0;
	virtual bool IsInt() = 0;
	virtual bool IsNil() = 0;

	virtual int GetTableSize(int stackIndex) = 0;
	virtual void IterateOverTable(int stackIndex, std::function<void(void)> iterateAction) = 0;

	virtual LuaResultWithValue<int>& ReadInteger(int stackIndex) = 0;
	virtual LuaResultWithValue<double>& ReadDouble(int stackIndex) = 0;
	virtual LuaResultWithValue<bool>& ReadBool(int stackIndex) = 0;
	virtual LuaResultWithValue<const char*>& ReadString(int stackIndex) = 0;

	virtual LuaResultWithValue<int>& ReadInteger() = 0;
	virtual LuaResultWithValue<double>& ReadDouble() = 0;
	virtual LuaResultWithValue<bool>& ReadBool() = 0;
	virtual LuaResultWithValue<const char*>& ReadString() = 0;

	template<class T> std::vector<T>& ReadArray(int stackIndex)
	{
		if (!IsTable(stackIndex))
		{
			return *new std::vector<T>();
		}

		auto& array = *new std::vector<T>(GetTableSize(stackIndex));
		auto hasError = false;

		IterateOverTable(stackIndex, [&]() {
			if (hasError)
			{
				return;
			}

			auto& idxResult = ReadInteger(-1);
			auto& valueResult = PullValue<T>(-2);

			if (!idxResult.IsErrorResult() && !valueResult.IsErrorResult())
			{
				auto vectorIdx = idxResult.GetValue() - 1;

				array[vectorIdx] = valueResult.GetValue();
			}
			else
			{
				hasError = true;
			}

			delete &idxResult;
			delete &valueResult;
		});

		return array;
	}

	template<class T> std::vector<T>& ReadArray()
	{
		return ReadArray<T>(GetStackTop());
	}

	template<class T> std::map<const char*, T>& ReadObject(int stackIndex)
	{
		auto& object = *new std::map<const char*, T>();
		auto hasError = false;

		if (!IsTable(stackIndex))
		{
			return object;
		}

		IterateOverTable([]() {
			if (hasError)
			{
				return;
			}

			auto& keyResult = ToString(-1);
			auto& valueResult = PullValue(-2);

			if (!keyResult.IsErrorResult() && !valueResult.IsErrorResult())
			{
				object[keyResult.GetValue()] = valueResult.GetValue();
			}
			else
			{
				hasError = true;
			}

			delete* keyResult;
			delete* valueResult;
		});

		return object;
	}

	template<class T> std::map<const char*, T>& ReadObject()
	{
		return ReadObject<T>(GetStackTop());
	}

	template<class T> LuaResultWithValue<T>& PullValue(int stackIndex)
	{
		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			return ReadString(stackIndex);
		}
		else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, unsigned char>
			|| std::is_same_v<T, short> || std::is_same_v<T, unsigned short>
			|| std::is_same_v<T, int> || std::is_same_v<T, unsigned int>
			|| std::is_same_v<T, long> || std::is_same_v<T, unsigned long>
			|| std::is_same_v<T, unsigned long long> || std::is_same_v<T, long long>)
		{
			return ReadInteger(stackIndex);
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, unsigned double>
			|| std::is_same_v<T, float> || std::is_same_v<T, unsigned float>)
		{
			return ReadDouble(stackIndex);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return ReadBool(stackIndex);
		}
		else
		{
			return LuaResultWithValue<void*>::BuildWithValue(NULL);
		}
	}

	template<class T> LuaResultWithValue<T>& PullValue()
	{
		return PullValue(GetStackTop());
	}

	virtual void WriteInteger(int value) = 0;
	virtual void WriteDouble(double value) = 0;
	virtual void WriteBool(bool value) = 0;
	virtual void WriteString(const char* value) = 0;
	virtual void WriteFunction(const char* name, lua_CFunction function) = 0;
	virtual void WriteNil() = 0;
	virtual void WriteTable(unsigned int expectedSize = 0) = 0;

	template<class T> void WriteArray(const std::vector<T>& values)
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

	template<class T> void WriteObject(const std::map<char*, T>& values)
	{
		lua_createtable(lua, 0, values.size());

		for (auto const& [k, v] : values)
		{
			PushValue<T>(v);

			SetIndex(-2, k);
		}
	}

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

	virtual void ClearStack() = 0;

	virtual void RaiseError(const char* messageFormat, ...) = 0;

	virtual LuaResult& ExecuteScript(const char* script) = 0;
	virtual LuaResult& ExecuteFile(const char* filePath) = 0;
};
