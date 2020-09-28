#include "LuaResult.h"

template<class T> class LuaResultWithValue : public LuaResult
{
private:
	T value;

	LuaResultWithValue(T value, const char* error = NULL) : LuaResult(error), value(value)
	{
	}

public:
	static LuaResultWithValue& BuildWithValue(T value)
	{
		return *(new LuaResultWithValue<T>(value));
	}

	static LuaResultWithValue& BuildWithError(T defaultValue, const char* error)
	{
		return *(new LuaResultWithValue<T>(defaultValue, error));
	}

	T GetValue()
	{
		return value;
	}
};
