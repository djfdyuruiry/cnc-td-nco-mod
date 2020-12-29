#pragma once

#include <map>

#include <HashUtils.h>

class LuaType final
{
private:
	LuaType(const char* value) : value(value)
	{
	}

public:
	static const LuaType& Number;
	static const LuaType& Bool;
	static const LuaType& String;
	static const LuaType& Function;
	static const LuaType& Table;
	static const LuaType& Nil;
	static const LuaType& Any;

	static const std::map<StringHash, const LuaType*>& NameToType;

	const char* value;

	static LuaType& Build(const char* typeName)
	{
		return *(new LuaType(typeName));
	}

	static const LuaType& Parse(const char* typeName);

	static bool AreEqual(const LuaType& expected, const LuaType& actual);

	static bool LuaType::AreEqual(const LuaType& expected, LuaType* actual);
};
