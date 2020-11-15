#include "LuaType.h"

LuaType* LuaType::Number;
LuaType* LuaType::Bool;
LuaType* LuaType::String;
LuaType* LuaType::Function;
LuaType* LuaType::Table;
LuaType* LuaType::Nil;
LuaType* LuaType::Any;

std::map<StringHash, LuaType*>* LuaType::NameToType;

const LuaType& LuaType::Parse(const char* typeName)
{
	InitIfRequired();

	auto key = HashUtils::HashString(typeName);
	auto& index = NameToType->find(key);

	if (index == NameToType->end())
	{
		Log_Error("Failed to parse lua type: %s", typeName);

		return *LuaType::Any;
	}

	return *index->second;
}

void LuaType::InitIfRequired()
{
	if (Number != NULL)
	{
		return;
	}

	Number = new LuaType("number");
	Bool = new LuaType("bool");
	String = new LuaType("string");
	Function = new LuaType("function");
	Table = new LuaType("table");
	Nil = new LuaType("nil");
	Any = new LuaType("any");

	NameToType = new std::map<StringHash, LuaType*>;

	auto& nameToType = *NameToType;

	nameToType[HashUtils::HashString("number")] = LuaType::Number;
	nameToType[HashUtils::HashString("boolean")] = LuaType::Bool;
	nameToType[HashUtils::HashString("string")] = LuaType::String;
	nameToType[HashUtils::HashString("function")] = LuaType::Function;
	nameToType[HashUtils::HashString("table")] = LuaType::Table;
	nameToType[HashUtils::HashString("nil")] = LuaType::Nil;
	nameToType[HashUtils::HashString("any")] = LuaType::Any;
}
