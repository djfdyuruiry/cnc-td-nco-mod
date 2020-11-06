#pragma once

#include "PrimitiveTypeValidator.h"

template<class T> class NumbericRangeValidator : public PrimitiveTypeValidator<T>
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

	bool IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		if (!PrimitiveTypeValidator::IsValid(lua, stackIndex))
		{
			return false;
		}

		auto& valueResult = lua.PullValue<T>(stackIndex);

		if (valueResult.IsErrorResult())
		{
			delete &valueResult;

			return false;
		}

		auto value = valueResult.GetValue();
	
		delete &valueResult;

		return value >= minInclusive && value <= maxInclusive;
	}
};
