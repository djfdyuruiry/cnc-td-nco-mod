#pragma once

#include <functional>
#include <map>
#include <type_traits>
#include <vector>

#include <lua.hpp>

#include <Result.h>
#include <ResultWithValue.h>

#include "LuaType.h"

class ILuaStateWrapper
{
protected:
	virtual void PushOntoStack(int stackIndex) = 0;
	virtual void PopFromStack(int stackIndex) = 0;

	virtual void SetIndex(int tableIndex, int index) = 0;
	virtual void SetIndex(int tableIndex, const char* index) = 0;
	virtual bool PopKeyValuePair(int tableIndex) = 0;

public:
	virtual int GetStackTop() = 0;
	virtual int GetUpvalueStackIndex(int upvalueIndex) = 0;
	virtual const char* GetLastError() = 0;

	virtual const char* ToString(int stackIndex) = 0;
	virtual const char* ToString() = 0;

	virtual const char* GetType(int stackIndex) = 0;
	virtual const char* GetType() = 0;

	virtual const LuaType& GetLuaType(int stackIndex) = 0;
	virtual const LuaType& GetLuaType() = 0;

	virtual bool IsString(int stackIndex) = 0;
	virtual bool IsTable(int stackIndex) = 0;
	virtual bool IsBool(int stackIndex) = 0;
	virtual bool IsInt(int stackIndex) = 0;
	virtual bool IsNumber(int stackIndex) = 0;
	virtual bool IsNil(int stackIndex) = 0;

	virtual bool IsString() = 0;
	virtual bool IsTable() = 0;
	virtual bool IsBool() = 0;
	virtual bool IsInt() = 0;
	virtual bool IsNumber() = 0;
	virtual bool IsNil() = 0;

	virtual int GetTableSize(int stackIndex) = 0;

	virtual Result& IterateOverTable(int stackIndex, std::function<void(int, int, const LuaType&)> iterateAction) = 0;

	virtual Result& IterateOverTable(std::function<void(int, int, const LuaType&)> iterateAction) = 0;
	
	template<class T> Result& IterateOverTable(int stackIndex, std::function<void(T, int, const LuaType&)> iterateAction)
	{
		if (!IsTable(stackIndex))
		{
			return Result::BuildWithError("Value at stack index '%d' was not a table", stackIndex);
		}

		PushOntoStack(stackIndex);

		WriteNil();

		auto keyIndex = -2;
		auto valIndex = -1;

		while (PopKeyValuePair(-2))
		{
			auto& keyResult = PullValue<T>(keyIndex);

			if (keyResult.IsErrorResult())
			{
				delete &keyResult;

				if constexpr (std::is_same_v<T, const char*>)
				{
					return Result::BuildWithError("Unable to read table key");
				}
				else if constexpr (std::is_same_v<T, int>)
				{
					return Result::BuildWithError("Unable to read table index");
				}
				else
				{
					return Result::BuildWithError("Unable to read table key/index");
				}
			}

			auto key = keyResult.GetValue();
			auto valType = GetLuaType(valIndex);

			delete &keyResult;

			iterateAction(key, valIndex, valType);

			PopFromStack(1);
		}

		PopFromStack(1);

		return Result::Build();
	}

	template<class T> Result& IterateOverTable(std::function<void(T, int, const LuaType&)> iterateAction)
	{
		return IterateOverTable<T>(GetStackTop(), iterateAction);
	}

	virtual Result& PushGlobalOntoStack(const char* variable) = 0;

	virtual Result& PushTableFieldOntoStack(int stackIndex, const char* field) = 0;

	virtual Result& PushTableFieldOntoStack(const char* field) = 0;

	template<class T> ResultWithValue<T>& ReadTableField(int stackIndex, const char* field)
	{
		auto& fieldResult = PushTableFieldOntoStack(stackIndex, field);

		if (!fieldResult)
		{
			fieldResult.c
		}

		return PullValue<T>();
	}

	virtual ResultWithValue<int>& ReadInteger(int stackIndex) = 0;
	virtual ResultWithValue<long long>& ReadBigInteger(int stackIndex) = 0;
	virtual ResultWithValue<double>& ReadDouble(int stackIndex) = 0;
	virtual ResultWithValue<bool>& ReadBool(int stackIndex) = 0;
	virtual ResultWithValue<const char*>& ReadString(int stackIndex) = 0;
	virtual ResultWithValue<void*>& ReadUserData(int stackIndex) = 0;

	virtual ResultWithValue<int>& ReadInteger() = 0;
	virtual ResultWithValue<long long>& ReadBigInteger() = 0;
	virtual ResultWithValue<double>& ReadDouble() = 0;
	virtual ResultWithValue<bool>& ReadBool() = 0;
	virtual ResultWithValue<const char*>& ReadString() = 0;
	virtual ResultWithValue<void*>& ReadUserData() = 0;

	template<class T> std::vector<T>& ReadArray(int stackIndex)
	{
		if (!IsTable(stackIndex))
		{
			return *new std::vector<T>();
		}

		auto& array = *new std::vector<T>(GetTableSize(stackIndex));
		auto hasError = false;

		auto& tableResult = IterateOverTable<int>(stackIndex, [&](auto key, auto valueIndex, auto& _) {
			if (hasError)
			{
				return;
			}

			auto& valueResult = PullValue<T>(valueIndex);

			if (!valueResult.IsErrorResult())
			{
				auto vectorIdx = key - 1;

				array[vectorIdx] = valueResult.GetValue();
			}
			else
			{
				hasError = true;
			}

			delete &valueResult;
		});

		delete &tableResult;

		return array;
	}

	template<class T> std::vector<T>& ReadArray()
	{
		return ReadArray<T>(GetStackTop());
	}

	template<class T> ResultWithValue<T>& PullValue(int stackIndex)
	{
		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			auto& result = ReadString(stackIndex);
			auto& convertedResult = result.ConvertType<T>();

			delete &result;

			return convertedResult;
		}
		else if constexpr (
			std::is_same_v<T, char> || std::is_same_v<T, unsigned char>
			|| std::is_same_v<T, short> || std::is_same_v<T, unsigned short>
			|| std::is_same_v<T, int> || std::is_same_v<T, unsigned int>
		)
		{
			auto& result = ReadInteger(stackIndex);
			auto& convertedResult = result.ConvertType<T>();

			delete &result;

			return convertedResult;
		}
		else if constexpr (
			std::is_same_v<T, long> || std::is_same_v<T, unsigned long>
			|| std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>
		)
		{
			auto& result = ReadBigInteger(stackIndex);
			auto& convertedResult = result.ConvertType<T>();

			delete &result;

			return convertedResult;
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
		{
			auto& result = ReadDouble(stackIndex);
			auto& convertedResult = result.ConvertType<T>();

			delete &result;

			return convertedResult;
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return ReadBool(stackIndex);
		}
		else
		{
			return ResultWithValue<T>::BuildWithError("No read method defined for value type");
		}
	}

	template<class T> ResultWithValue<T>& PullValue()
	{
		return PullValue(GetStackTop());
	}

	template<class T> void PullOptionalValue(int stackIndex, std::function<void(T)> valueHandler)
	{
		auto& valueResult = PullValue<T>(stackIndex);

		if (valueResult.IsErrorResult())
		{
			delete& valueResult;

			return;
		}

		valueHandler(valueResult.GetValue());

		delete& valueResult;
	}

	template<class T> void PullOptionalValue(std::function<void(T)> valueHandler)
	{
		return PullOptionalValue<T>(GetStackTop(), valueHandler);
	}

	virtual void WriteInteger(int value) = 0;
	virtual void WriteBigInteger(long long value) = 0;
	virtual void WriteNumber(double value) = 0;
	virtual void WriteBool(bool value) = 0;
	virtual void WriteString(const char* value) = 0;
	virtual void WriteFunction(lua_CFunction function, void* functionInfo) = 0;
	virtual void WriteMethod(void* objectPtr, lua_CFunction methodProxy, void* functionInfo) = 0;
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

	template<class T, class U> void PushTableEntry(T key, U value, int tableIndex = -2)
	{
		PushValue<U>(value);

		SetIndex(tableIndex, key);
	}

	template<class T> void SetTableIndex(T key, int tableIndex = -2)
	{
		SetIndex(tableIndex, key);
	}

	template<class T> void PushValue(T value)
	{
		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			WriteString(value);
		}
		else if constexpr (
			std::is_same_v<T, char> || std::is_same_v<T, unsigned char>
			|| std::is_same_v<T, short> || std::is_same_v<T, unsigned short>
			|| std::is_same_v<T, int> || std::is_same_v<T, unsigned int>
		)
		{
			WriteInteger(value);
		}
		else if constexpr (
			std::is_same_v<T, long> || std::is_same_v<T, unsigned long>
			|| std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>
			)
		{
			WriteBigInteger(value);
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
		{
			WriteNumber(value);
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
	virtual void RaiseError(Result& result) = 0;

	virtual Result& ExecuteScript(const char* script) = 0;
	virtual Result& ExecuteFile(const char* filePath) = 0;

};
