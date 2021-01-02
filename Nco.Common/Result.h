#pragma once

#include <stdarg.h>
#include <stdlib.h>

#include "strings.h"

class Result
{
private:
	char* error;

protected:
	Result(char* error = NULL) : error(error)
	{
	}

public:
	static Result& Build()
	{
		return *(new Result());
	}

	static Result& BuildWithError(const char* error, ...)
	{
		va_list formatArgs;
		va_start(formatArgs, error);

		auto& instance = *(new Result(FormatString(error, formatArgs)));

		va_end(formatArgs);

		return instance;
	}

	~Result()
	{
		if (error != NULL)
		{
			delete error;
		}
	}

	bool IsErrorResult()
	{
		return !StringIsEmpty(error);
	}

	const char* GetError()
	{
		return error;
	}

};
