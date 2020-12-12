#pragma once

#include "LuaType.h"

class LuaVariableInfo
{
private:
	char* name;
	char* description;
	LuaType* type;
	
	bool isParameter;
	bool isOptional;
	bool isReturnValue;

	unsigned int parameterIndex;
	unsigned int returnValueIndex;

	LuaVariableInfo() :
		name(NULL),
		description(NULL),
		type(LuaType::Any),
		isParameter(false),
		isReturnValue(false),
		parameterIndex(0),
		returnValueIndex(0)
	{
	}

public:
	static LuaVariableInfo& Build()
	{
		return *(new LuaVariableInfo());
	}

	static LuaVariableInfo& BuildParameter(unsigned int index, bool isOptional = false)
	{
		auto& instance = *(new LuaVariableInfo());

		instance.isParameter = true;
		instance.isOptional = isOptional;
		instance.parameterIndex = index;

		return instance;
	}

	static LuaVariableInfo& BuildReturnValue(unsigned int index)
	{
		auto& instance = *(new LuaVariableInfo());

		instance.isReturnValue = true;
		instance.returnValueIndex = index;

		return instance;
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

	LuaVariableInfo& WithType(LuaType* type)
	{
		this->type = type;

		return *this;
	}

	LuaVariableInfo& WithType(const LuaType& type)
	{
		this->type = &((LuaType&)type);

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

	bool IsParameter()
	{
		return isParameter;
	}

	bool IsOptional()
	{
		return isOptional;
	}

	bool IsReturnValue()
	{
		return isReturnValue;
	}

	unsigned int GetParameterIndex()
	{
		return parameterIndex;
	}

	unsigned int GetReturnValueIndex()
	{
		return returnValueIndex;
	}

};
