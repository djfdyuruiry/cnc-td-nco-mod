#pragma once

#include <TypePatterns.h>
#include <strings.h>

#include "ILuaValueValidator.h"
#include <Result.h>

template<class T> class ParseCheckValidator : public ILuaValueValidator
{
protected:
	char* titleCaseTypeName;
	std::function<ResultWithValue<T>* (const char*)> parser;

	ParseCheckValidator(const char* typeName, std::function<ResultWithValue<T>*(const char*)> parser) : titleCaseTypeName(ToTitleCase(typeName)), parser(parser)
	{
	}

public:
	static ParseCheckValidator& Build(const char* typeName, std::function<ResultWithValue<T>*(const char*)> parser)
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

	Result& IsValid(ILuaStateWrapper& lua, int stackIndex)
	{
		auto& stringResult = lua.ReadString(stackIndex);

		if (stringResult.IsErrorResult())
		{
			return stringResult;
		}

		auto value = stringResult.GetValue();
		auto valueUpper = ConvertStringToUpperCase(value);

		delete &stringResult;

		auto& parseResult = *parser(valueUpper);

		delete valueUpper;

		if (parseResult.IsErrorResult())
		{
			auto error = FormatString(
				"Unable to parse string '%s' as %s type instance: %s",
				value,
				titleCaseTypeName,
				parseResult.GetError()
			);

			delete &parseResult;

			return Result::BuildWithError(error);
		}

		delete &parseResult;

		return Result::Build();
	}

};
