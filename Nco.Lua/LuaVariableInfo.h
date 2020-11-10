#pragma once

#include "LuaType.h"

class LuaVariableInfo
{
private:
	const char* name;
	const char* description;
	LuaType* type;

	LuaVariableInfo() : type((LuaType*)&LuaType::Nil)
	{
	}

public:
	static LuaVariableInfo& Build()
	{
		return *(new LuaVariableInfo());
	}

	LuaVariableInfo& WithName(const char* name)
	{
		this->name = name;

		return *this;
	}

	LuaVariableInfo& WithDescription(const char* description)
	{
		this->description = description;

		return *this;
	}

	LuaVariableInfo& WithType(const LuaType& type)
	{
		this->type = (LuaType*)&type;

		return *this;
	}

	const char* GetName()
	{
		return name;
	}

	const char* GetDescription()
	{
		return description;
	}

	const LuaType& GetType()
	{
		return *type;
	}

};
