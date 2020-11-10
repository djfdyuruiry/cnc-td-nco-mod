#pragma once

#include "ILuaStateWrapper.h"
#include "ILuaValueValidator.h"
#include "LuaResultWithValue.h"

template<class T> class LuaValueUtils
{
public:
	static LuaResultWithValue<T> ValidateAndReadValue(
		ILuaStateWrapper& lua,
		int stackIndex,
		ILuaValueValidator& validator
	)
	{
		if (!validator.IsValid(lua, stackIndex))
		{
			return LuaResultWithValue<T>::BuildWithError("Lua value provided was invalid");
		}

		auto value = lua.PullValue<T>(stackIndex);

		return LuaResultWithValue<T>::Build(value);
	}

};
