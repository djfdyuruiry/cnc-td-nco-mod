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

	static LuaResult& Build(char* error)
	{
		return *(new LuaResult(error));
	}

	static LuaResult& Build(const char* error)
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
		return !String_Is_Empty(error);
	}

	const char* GetError()
	{
		return error;
	}
};
