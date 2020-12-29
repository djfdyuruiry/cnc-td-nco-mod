#pragma once

#include <lua.hpp>

#include <Result.h>

#include "PrimitiveTypeValidator.h"

template<class T = LUA_INTEGER> class NumbericRangeValidator : public PrimitiveTypeValidator<T>
{
private:
	T minInclusive;
	T maxInclusive;

	NumbericRangeValidator(T minInclusive, T maxInclusive) : minInclusive(minInclusive), maxInclusive(maxInclusive)
	{
	}

public:
	static ILuaValueValidator& Build(T minInclusive, T maxInclusive)
	{
		return *(new NumbericRangeValidator(minInclusive, maxInclusive));
	}

	Result& IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& primitiveResult = PrimitiveTypeValidator::IsValid(lua, stackIndex);

		if (primitiveResult.IsErrorResult())
		{
			return primitiveResult;
		}

		delete &primitiveResult;

		auto& valueResult = lua.PullValue<T>(stackIndex);

		if (valueResult.IsErrorResult())
		{
			return valueResult;
		}

		auto value = valueResult.GetValue();
	
		delete &valueResult;

		if (value < minInclusive && value > maxInclusive)
		{
			return Result::BuildWithError("Number must be in the range %d-%d (inclusive)", minInclusive, maxInclusive);
		}

		return Result::Build();
	}

};
