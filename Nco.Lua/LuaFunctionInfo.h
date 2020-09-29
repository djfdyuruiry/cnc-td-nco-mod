#pragma once

#include <vector>

#include "LuaVariableInfo.h"

typedef void (*LuaVariableInfoInitialiser)(LuaVariableInfo&);

class LuaFunctionInfo
{
private:
	const char* name;
	const char* description;
	std::vector<LuaVariableInfo*>& parameters;
	std::vector<LuaVariableInfo*>& returnValues;

	LuaFunctionInfo() : parameters(*(new std::vector<LuaVariableInfo*>())),
		returnValues(*(new std::vector<LuaVariableInfo*>()))
	{
	}

	LuaVariableInfo& BuildVarInfo(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		auto& varInfo = LuaVariableInfo::Build().WithName(name);

		if (initialiser != NULL)
		{
			initialiser(varInfo);
		}

		return varInfo;
	}

public:
	static LuaFunctionInfo& Build()
	{
		return *(new LuaFunctionInfo());
	}

	LuaFunctionInfo& WithName(const char* name)
	{
		this->name = name;

		return *this;
	}

	LuaFunctionInfo& WithDescription(const char* description)
	{
		this->description = description;

		return *this;
	}

	LuaFunctionInfo& WithParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		parameters.push_back(
			&BuildVarInfo(name, initialiser)
		);

		return *this;
	}

	LuaFunctionInfo& WithReturnValue(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		returnValues.push_back(
			&BuildVarInfo(name, initialiser)
		);

		return *this;
	}
};
