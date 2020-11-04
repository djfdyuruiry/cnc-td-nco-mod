#pragma once

#include <ILuaValueValidator.h>

#include "parse.h"

class StructTypeValidator : public ILuaValueValidator
{
protected:
	StructTypeValidator()
	{
	}

public:
	static ILuaValueValidator& Build()
	{
		return *(new StructTypeValidator());
	}

	bool IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& result = lua.ReadString(stackIndex);
		bool isValid = false;

		if (!result.IsErrorResult())
		{
			Parse_Structure_Type(result.GetValue(), &isValid);
		}
	
		delete &result;

		return isValid;
	}
};
