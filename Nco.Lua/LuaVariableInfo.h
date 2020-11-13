#pragma once

#include "LuaType.h"

class LuaVariableInfo
{
private:
	char* name;
	char* description;
	LuaType* type;

	LuaVariableInfo() : type((LuaType*)&LuaType::Nil)
	{
	}

public:
	static LuaVariableInfo& Build()
	{
		return *(new LuaVariableInfo());
	}

	~LuaVariableInfo()
	{
		if (name != NULL)
		{
			delete name;
		}

		if (description != NULL)
		{
			delete description;
		}
	}

	LuaVariableInfo& WithName(char* name)
	{
		this->name = name;

		return *this;
	}

	LuaVariableInfo& WithName(const char* name)
	{
		return WithName(strdup(name));
	}

	LuaVariableInfo& WithDescription(char* description)
	{
		this->description = description;

		return *this;
	}

	LuaVariableInfo& WithDescription(const char* description)
	{
		return WithDescription(strdup(description));
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
