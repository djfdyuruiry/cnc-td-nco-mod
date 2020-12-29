#include <HashUtils.h>
#include <logger.h>
#include <strings.h>

#include "LuaType.h"

const LuaType& LuaType::Number = LuaType::Build("number");
const LuaType& LuaType::Bool = LuaType::Build("bool");
const LuaType& LuaType::String = LuaType::Build("string");
const LuaType& LuaType::Function = LuaType::Build("function");
const LuaType& LuaType::Table = LuaType::Build("table");
const LuaType& LuaType::Nil = LuaType::Build("nil");
const LuaType& LuaType::Any = LuaType::Build("any");

const std::map<StringHash, const LuaType*>& LuaType::NameToType = std::map<StringHash, const LuaType*>
{
	{ HashUtils::HashString(LuaType::Number.value), &LuaType::Number },
	{ HashUtils::HashString(LuaType::Bool.value), &LuaType::Bool },
	{ HashUtils::HashString(LuaType::String.value), &LuaType::String },
	{ HashUtils::HashString(LuaType::Function.value), &LuaType::Function },
	{ HashUtils::HashString(LuaType::Table.value), &LuaType::Table },
	{ HashUtils::HashString(LuaType::Nil.value), &LuaType::Nil },
	{ HashUtils::HashString(LuaType::Any.value), &LuaType::Any }
};

const LuaType& LuaType::Parse(const char* typeName)
{
	auto key = HashUtils::HashString(typeName);
	auto& index = NameToType.find(key);

	if (index == NameToType.end())
	{
		LogError("Failed to parse lua type: %s", typeName);

		return LuaType::Any;
	}

	return *index->second;
}

bool LuaType::AreEqual(const LuaType& expected, const LuaType& actual)
{
	return StringsAreEqual(expected.value, actual.value);
}

bool LuaType::AreEqual(const LuaType& expected, LuaType* actual)
{
	return StringsAreEqual(expected.value, actual->value);
}
