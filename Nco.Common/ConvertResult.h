#pragma once

#include <stdlib.h>
#include <string.h>

template <class T> class ConvertResult
{
private:
	T value;
	char* error;
	bool isErrorResult;

	ConvertResult(T value) : value(value), error(NULL), isErrorResult(false)
	{
	}

	ConvertResult(char* error) : error(error), isErrorResult(true)
	{
	}

public:
	static ConvertResult<T>& BuildWithValue(T value)
	{
		return *(new ConvertResult<T>(value));
	}

	static ConvertResult<T>& BuildWithError(char* error)
	{
		return *(new ConvertResult<T>(error));
	}

	static ConvertResult<T>& BuildWithError(const char* error)
	{
		return BuildWithError(strdup(error));
	}

	~ConvertResult()
	{
		if (error != NULL)
		{
			delete error;
		}
	}

	T GetValue()
	{
		return value;
	}

	bool IsErrorResult()
	{
		return isErrorResult;
	}

	const char* GetError()
	{
		return error;
	}

};
