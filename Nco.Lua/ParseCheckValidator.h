#pragma once

#include <TypePatterns.h>
#include <strings.h>

#include "ILuaValueValidator.h"
#include "LuaResult.h"

template<class T> class ParseCheckValidator : public ILuaValueValidator
{
protected:
	char* titleCaseTypeName;
	PARSER(T, parser);

	ParseCheckValidator(const char* typeName, PARSER(T, parser)) : titleCaseTypeName(ToTitleCase(typeName)), parser(parser)
	{
	}

public:
	static ParseCheckValidator& Build(const char* typeName, PARSER(T, parser))
	{
		return *(new ParseCheckValidator(typeName, parser));
	}

	~ParseCheckValidator()
	{
		if (titleCaseTypeName != NULL)
		{
			delete titleCaseTypeName;
		}
	}

	LuaResult& IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& result = lua.ReadString(stackIndex);

		if (result.IsErrorResult())
		{
			return result;
		}

		bool parseError = false;
		auto valueUpper = Convert_String_To_Upper_Case(result.GetValue());

		delete &result;

		parser(valueUpper, &parseError, false);

		if (parseError)
		{
			auto error = FormatString("Unable to parse string as %s type instance: %s", titleCaseTypeName, valueUpper);

			delete valueUpper;

			return LuaResult::Build(error);
		}

		delete valueUpper;

		return LuaResult::Build();
	}

};
