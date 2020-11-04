#pragma once

#include "ILuaValueValidator.h"

template<class T> class PrimitiveTypeValidator : public ILuaValueValidator
{
protected:
	PrimitiveTypeValidator()
	{
	}

public:
	static ILuaValueValidator& Build()
	{
		return *(new PrimitiveTypeValidator());
	}

	bool IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			return lua.IsString(stackIndex);
		}
		else if constexpr (
			std::is_same_v<T, char> || std::is_same_v<T, unsigned char>
			|| std::is_same_v<T, short> || std::is_same_v<T, unsigned short>
			|| std::is_same_v<T, int> || std::is_same_v<T, unsigned int>
			|| std::is_same_v<T, long> || std::is_same_v<T, unsigned long>
			|| std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>
		)
		{
			return lua.IsInt(stackIndex);
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
		{
			return lua.IsNumber(stackIndex);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return lua.IsBool(stackIndex);
		}
		else
		{
			return false;
		}
	}
};
