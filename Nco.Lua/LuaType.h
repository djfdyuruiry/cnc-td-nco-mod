#pragma once

#include <map>

#include <Logger.h>
#include <HashUtils.h>

class LuaType final
{
private:
	LuaType(const char* value) : value(value)
	{
	}

public:
	const char* value;

	static LuaType* Number;
	static LuaType* Bool;
	static LuaType* String;
	static LuaType* Function;
	static LuaType* Table;
	static LuaType* Nil;
	static LuaType* Any;

	static std::map<StringHash, LuaType*>* NameToType;

	static const LuaType& Parse(const char* typeName);

	static void InitIfRequired();
};
