#pragma once

#include "Result.h"
#include "TypeUtils.h"

template<class T> class ResultWithValue : public Result
{
private:
	T value;

	ResultWithValue(T value, char* error = NULL) : Result(error), value(value)
	{
	}

public:
	static ResultWithValue& BuildWithValue(T value)
	{
		return *(new ResultWithValue<T>(value));
	}

	static ResultWithValue& BuildWithError()
	{
		return *(new ResultWithValue<T>(TypeUtils::GetDefaultValue<T>()));
	}

	static ResultWithValue& BuildWithError(char* error)
	{
		return *(new ResultWithValue<T>(TypeUtils::GetDefaultValue<T>(), error));
	}

	static ResultWithValue& BuildWithError(const char* error, ...)
	{
		va_list formatArgs;
		va_start(formatArgs, error);

		auto& instance = BuildWithError(FormatString(error, formatArgs));

		va_end(formatArgs);

		return instance;
	}

	T GetValue()
	{
		return value;
	}

	template<class U> ResultWithValue<U>& ConvertType()
	{
		if (IsErrorResult())
		{
			return ResultWithValue<U>::BuildWithError(GetError());
		}

		return ResultWithValue<U>::BuildWithValue(GetValue());
	}

};
