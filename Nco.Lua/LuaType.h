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
	const char* value;

	static const LuaType& Number;
	static const LuaType& Bool;
	static const LuaType& String;
	static const LuaType& Function;
	static const LuaType& Table;
	static const LuaType& Nil;
	static const LuaType& Any;

	static const std::map<StringHash, const LuaType&>& NameToType;

	static const LuaType& Parse(const char* typeName)
	{
		auto key = HashUtils::HashString(typeName);
		auto& index = NameToType.find(key);

		if (index == NameToType.end())
		{
			return NULL;
		}

		return index->second;
	}

};
