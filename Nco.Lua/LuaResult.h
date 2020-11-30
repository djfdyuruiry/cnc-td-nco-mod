#pragma once

#include <stdlib.h>

#include <lua.hpp>
#include <strings.h>

class LuaResult
{
private:
	char* error;

protected:
	LuaResult(char* error = NULL) : error(error)
	{
	}

public:
	static LuaResult& Build()
	{
		return *(new LuaResult());
	}

	static LuaResult& BuildWithError(const char* error, ...)
	{
		va_list formatArgs;
		va_start(formatArgs, error);

		auto& instance = *(new LuaResult(FormatString(error, formatArgs)));

		va_end(formatArgs);

		return instance;
	}

	~LuaResult()
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
