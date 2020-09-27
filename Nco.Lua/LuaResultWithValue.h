#include "LuaResult.h"

template<class T> class LuaResultWithValue : public LuaResult
{
protected:
	LuaResultWithValue(const char* error) : LuaResult(error)
	{
	}

	LuaResultWithValue(T value) : LuaResult()
	{
	}

public:
	static template<class U> LuaResultWithValue& Build(T value)
	{
		return *(new LuaResultWithValue<U>(value));
	}

	static template<class U> LuaResultWithValue& Build(const char* error)
	{
		return *(new LuaResultWithValue<U>(error));
	}
};
