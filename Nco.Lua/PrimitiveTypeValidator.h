#pragma once

#include "ILuaValueValidator.h"
#include "LuaResult.h"

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

	LuaResult& IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto isValid = false;
		const char* error;

		if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
		{
			isValid = lua.IsString(stackIndex);
			error = "Value must be a string";
		}
		else if constexpr (
			std::is_same_v<T, char> || std::is_same_v<T, unsigned char>
			|| std::is_same_v<T, short> || std::is_same_v<T, unsigned short>
			|| std::is_same_v<T, int> || std::is_same_v<T, unsigned int>
			|| std::is_same_v<T, long> || std::is_same_v<T, unsigned long>
			|| std::is_same_v<T, long long> || std::is_same_v<T, unsigned long long>
		)
		{
			isValid = lua.IsInt(stackIndex);
			error = "Value must be an integer number";
		}
		else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
		{
			isValid = lua.IsNumber(stackIndex);
			error = "Value must be an number";
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			isValid = lua.IsBool(stackIndex);
			error = "Value must be a bool";
		}
		else
		{
			error = "C++ type not matched for lua type validation";
		}

		if (isValid)
		{
			return LuaResult::Build();
		}

		return LuaResult::BuildWithError(error);
	}

};
