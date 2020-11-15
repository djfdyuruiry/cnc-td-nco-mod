#include "LuaType.h"

const LuaType& LuaType::Number = LuaType("number");
const LuaType& LuaType::Bool = LuaType("bool");
const LuaType& LuaType::String = LuaType("string");
const LuaType& LuaType::Function = LuaType("function");
const LuaType& LuaType::Table = LuaType("table");
const LuaType& LuaType::Nil = LuaType("nil");
const LuaType& LuaType::Any = LuaType("any");

const std::map<StringHash, const LuaType&>& LuaType::NameToType = {
	{ HashUtils::HashString("number"), LuaType::Number },
	{ HashUtils::HashString("boolean"), LuaType::Bool },
	{ HashUtils::HashString("string"), LuaType::String },
	{ HashUtils::HashString("function"), LuaType::Function },
	{ HashUtils::HashString("table"), LuaType::Table },
	{ HashUtils::HashString("nil"), LuaType::Nil },
	{ HashUtils::HashString("any"), LuaType::Any }
};
