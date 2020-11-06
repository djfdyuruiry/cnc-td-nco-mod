#pragma once

#include <TypeUtils.h>

#include "LuaResult.h"

template<class T> class LuaResultWithValue : public LuaResult
{
private:
	T value;

	LuaResultWithValue(T value, char* error = NULL) : LuaResult(error), value(value)
	{
	}

public:
	static LuaResultWithValue& BuildWithValue(T value)
	{
		return *(new LuaResultWithValue<T>(value));
	}

	static LuaResultWithValue& BuildWithError(char* error)
	{
		return *(new LuaResultWithValue<T>(TypeUtils::GetDefaultValue<T>(), error));
	}

	static LuaResultWithValue& BuildWithError(const char* error)
	{
		return BuildWithError(strdup(error));
	}

	T GetValue()
	{
		return value;
	}

	template<class U> LuaResultWithValue<U>& ConvertType()
	{
		if (IsErrorResult())
		{
			return LuaResultWithValue<U>::BuildWithError(GetError());
		}

		return LuaResultWithValue<U>::BuildWithValue(GetValue());
	}
};
