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
		auto& validateResult = validator.IsValid(lua, stackIndex);

		if (validateResult.IsErrorResult())
		{
			auto& readResult = LuaResultWithValue<T>::BuildWithError(FormatString("Lua value provided was invalid: %s", result.GetError());

			delete &validateResult;

			return readResult;
		}

		auto value = lua.PullValue<T>(stackIndex);

		return LuaResultWithValue<T>::Build(value);
	}

};
