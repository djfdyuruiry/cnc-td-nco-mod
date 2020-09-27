#pragma once

#include <lua.hpp>
#include <stdlib.h>

class LuaResult
{
private:
	const char* error;

protected:
	LuaResult(const char* error = NULL) : error(error)
	{
	}

public:
	static LuaResult& Build(const char* error = NULL)
	{
		return *(new LuaResult(error));
	}

	bool IsErrorResult()
	{
		return error != NULL;
	}

	const char* GetError()
	{
		return error;
	}
};
