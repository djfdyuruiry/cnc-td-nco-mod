#pragma once

#include <vector>

#include <lua.hpp>

#include "LuaVariableInfo.h"

typedef void (*LuaVariableInfoInitialiser)(LuaVariableInfo&);

class LuaFunctionInfo
{
private:
	char* name;
	char* description;

	lua_CFunction luaFunction;

	void* implementationObject;
	lua_CFunction methodProxy;

	std::vector<LuaVariableInfo*>& parameters;
	std::vector<LuaVariableInfo*>& returnValues;

	LuaFunctionInfo() : parameters(*(new std::vector<LuaVariableInfo*>())),
		returnValues(*(new std::vector<LuaVariableInfo*>())),
		implementationObject(NULL)
	{
	}

	LuaVariableInfo& BuildVarInfo(char* name, LuaVariableInfoInitialiser initialiser)
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

	LuaFunctionInfo& WithName(char* name)
	{
		this->name = name;

		return *this;
	}

	LuaFunctionInfo& WithName(const char* name)
	{
		return WithName(strdup(name));
	}

	LuaFunctionInfo& WithImplementation(lua_CFunction impl)
	{
		this->luaFunction = impl;

		return *this;
	}

	LuaFunctionInfo& WithImplementation(void* object, lua_CFunction objectMethodProxy)
	{
		implementationObject = object;
		methodProxy = objectMethodProxy;

		return *this;
	}

	LuaFunctionInfo& WithDescription(char* description)
	{
		this->description = description;

		return *this;
	}

	LuaFunctionInfo& WithDescription(const char* description)
	{
		return WithDescription(strdup(name));
	}

	LuaFunctionInfo& WithParameter(char* name, LuaVariableInfoInitialiser initialiser)
	{
		parameters.push_back(
			&BuildVarInfo(name, initialiser)
		);

		return *this;
	}

	LuaFunctionInfo& WithParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithParameter(strdup(name), initialiser);
	}

	LuaFunctionInfo& WithReturnValue(char* name, LuaVariableInfoInitialiser initialiser)
	{
		returnValues.push_back(
			&BuildVarInfo(name, initialiser)
		);

		return *this;
	}

	LuaFunctionInfo& WithReturnValue(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithReturnValue(strdup(name), initialiser);
	}

	const char* GetName()
	{
		return name;
	}

	const char* GetDescription()
	{
		return description;
	}

	bool IsObjectMethod()
	{
		return implementationObject != NULL;
	}

	lua_CFunction GetFunction()
	{
		return luaFunction;
	}

	void* GetImplementationObject()
	{
		return implementationObject;
	}

	lua_CFunction GetMethodProxy()
	{
		return methodProxy;
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
