#pragma once

#include <ILuaValueValidator.h>

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
			parser(result.GetValue(), &parseError, false);
		}

		delete& result;

		return !parseError;
	}

};

