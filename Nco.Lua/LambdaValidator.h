#pragma once

#include <functional>

#include "ILuaValueValidator.h"
#include "LuaResult.h"

template<class T> class LambdaValidator : public ILuaValueValidator
{
protected:
	const char* failureMessage;
	std::function<bool(T)> validator;

	LambdaValidator(char* failureMessage, std::function<bool(T)> validator) : failureMessage(failureMessage), validator(validator)
	{
	}

public:
	static ILuaValueValidator& Build(char* failureMessage, std::function<bool(T)> validator)
	{
		return *(new LambdaValidator(failureMessage, validator));
	}

	static ILuaValueValidator& Build(const char* failureMessage, std::function<bool(T)> validator)
	{
		return Build(strdup(failureMessage), validator);
	}

	~LambdaValidator()
	{
		if (failureMessage != NULL)
		{
			delete failureMessage;
		}
	}

	LuaResult& IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& result = lua.PullValue<T>(stackIndex);

		if (result.IsErrorResult())
		{
			return result;
		}

		auto isValid = validator(result.GetValue());

		delete &result;

		if (isValid)
		{
			return LuaResult::Build();
		}

		return LuaResult::Build(failureMessage);
	}

};
