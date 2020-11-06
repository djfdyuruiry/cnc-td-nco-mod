#pragma once

#include <functional>

#include "ILuaValueValidator.h"

template<class T> class LambdaValidator : public ILuaValueValidator
{
protected:
	std::function<bool(T)> validator;

	LambdaValidator(std::function<bool(T)> validator) : validator(validator)
	{
	}

public:
	static ILuaValueValidator& Build(std::function<bool(T)> validator)
	{
		return *(new LambdaValidator(validator));
	}

	bool IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& result = lua.PullValue<T>(stackIndex);

		if (result.IsErrorResult())
		{
			delete &result;

			return false;
		}

		auto isValid = validator(result.GetValue());

		delete &result;

		return isValid;
	}

};
