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
		bool parseError = false;

		if (!result.IsErrorResult())
		{
			Parse_Structure_Type(result.GetValue(), &parseError);
		}
	
		delete &result;

		return !parseError;
	}
};
