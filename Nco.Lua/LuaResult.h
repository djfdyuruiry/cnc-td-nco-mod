#pragma once

#include <stdlib.h>

#include <lua.hpp>

class LuaResult
{
private:
	char* error;

protected:
	LuaResult(char* error = NULL) : error(error)
	{
	}

public:
	static LuaResult& Build(char* error = NULL)
	{
		return *(new LuaResult(error));
	}

	static LuaResult& Build(const char* error = NULL)
	{
		return Build(strdup(error));
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
		return error != NULL;
	}

	const char* GetError()
	{
		return error;
	}
};
