#pragma once

#include "ILuaStateWrapper.h"
#include "ILuaValueValidator.h"
#include <ResultWithValue.h>

template<class T> class LuaValueUtils
{
public:
	static ResultWithValue<T> ValidateAndReadValue(
		ILuaStateWrapper& lua,
		int stackIndex,
		ILuaValueValidator& validator
	)
	{
		auto& validateResult = validator.IsValid(lua, stackIndex);

		if (validateResult.IsErrorResult())
		{
			auto& readResult = ResultWithValue<T>::BuildWithError(FormatString("Lua value provided was invalid: %s", result.GetError());

			delete &validateResult;

			return readResult;
		}

		auto value = lua.PullValue<T>(stackIndex);

		return ResultWithValue<T>::Build(value);
	}

};
