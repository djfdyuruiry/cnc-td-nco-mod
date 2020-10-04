#pragma once

#include <vector>

#include <lua.hpp>

#include "LuaVariableInfo.h"

typedef void (*LuaVariableInfoInitialiser)(LuaVariableInfo&);

class LuaFunctionInfo
{
private:
	const char* name;
	const char* description;
	lua_CFunction luaFunction;
	std::vector<LuaVariableInfo*>& parameters;
	std::vector<LuaVariableInfo*>& returnValues;

	LuaFunctionInfo() : parameters(*(new std::vector<LuaVariableInfo*>())),
		returnValues(*(new std::vector<LuaVariableInfo*>()))
	{
	}

	LuaVariableInfo& BuildVarInfo(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		auto &varInfo = LuaVariableInfo::Build().WithName(name);

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

	~LuaFunctionInfo()
	{
		delete name;
		delete description;

		for (auto parameter : parameters) {
			delete parameter;
		}

		delete &parameters;

		for (auto returnValue : returnValues) {
			delete returnValue;
		}

		delete &returnValues;
	}

	LuaFunctionInfo& WithName(const char* name)
	{
		this->name = name;

		return *this;
	}

	LuaFunctionInfo& WithImplementation(lua_CFunction impl)
	{
		this->luaFunction = impl;

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

	const char* GetName()
	{
		return name;
	}

	const char* GetDescription()
	{
		return description;
	}

	lua_CFunction GetLuaFunction()
	{
		return luaFunction;
	}

	const std::vector<LuaVariableInfo*>& GetParameters()
	{
		return parameters;
	}

	const std::vector<LuaVariableInfo*>& GetReturnValues()
	{
		return returnValues;
	}

	int Call(lua_State* lua)
	{
		if (luaFunction == NULL)
		{
			return 0;
		}

		return luaFunction(lua);
	}
};
