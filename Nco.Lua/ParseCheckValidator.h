#pragma once

#include "ILuaValueValidator.h"

#define PARSER T(*parser)(const char*, bool*, bool)

template<class T> class ParseCheckValidator : public ILuaValueValidator
{
protected:
	PARSER;

	ParseCheckValidator(PARSER) : parser(parser)
	{
	}

public:
	static ParseCheckValidator& Build(PARSER)
	{
		return *(new ParseCheckValidator(parser));
	}

	bool IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& result = lua.ReadString(stackIndex);
		bool parseError = false;

		if (!result.IsErrorResult())
		{
			auto valueUpper = Convert_String_To_Upper_Case(result.GetValue());

			parser(valueUpper, &parseError, false);

			delete valueUpper;
		}

		delete& result;

		return !parseError;
	}

};
