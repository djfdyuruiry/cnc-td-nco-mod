#include <stdlib.h>

class LuaResult
{
private:
	const char* error;

protected:

	LuaResult(const char* error = NULL)
	{
		this->error = error;
	}

public:

	bool IsErrorResult()
	{
		return error != NULL;
	}

	const char* GetError()
	{
		return error;
	}
};
