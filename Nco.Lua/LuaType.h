#pragma once

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
};
