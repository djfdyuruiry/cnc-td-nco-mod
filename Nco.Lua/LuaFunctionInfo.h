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
	bool parameterValidationEnabled;

	lua_CFunction luaFunction;

	void* implementationObject;
	lua_CFunction methodProxy;

	std::vector<LuaVariableInfo*>& parameters;
	std::vector<LuaVariableInfo*>& returnValues;

	LuaFunctionInfo() : parameters(*(new std::vector<LuaVariableInfo*>())),
		returnValues(*(new std::vector<LuaVariableInfo*>())),
		implementationObject(NULL),
		parameterValidationEnabled(false)
	{
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

	LuaFunctionInfo& WithParameter(char* name, LuaVariableInfoInitialiser initialiser, bool isOptional = false)
	{
		auto& varInfo = LuaVariableInfo::BuildParameter(parameters.size() + 1)
			.WithName(name);

		if (initialiser != NULL)
		{
			initialiser(varInfo);
		}

		parameters.push_back(&varInfo);

		return *this;
	}

	LuaFunctionInfo& WithParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithParameter(strdup(name), initialiser);
	}

	LuaFunctionInfo& WithOptionalParameter(char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithParameter(strdup(name), initialiser, true);
	}

	LuaFunctionInfo& WithOptionalParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithOptionalParameter(strdup(name), initialiser);
	}

	LuaFunctionInfo& WithReturnValue(char* name, LuaVariableInfoInitialiser initialiser)
	{
		auto& varInfo = LuaVariableInfo::BuildReturnValue(returnValues.size() + 1)
			.WithName(name);

		if (initialiser != NULL)
		{
			initialiser(varInfo);
		}

		returnValues.push_back(&varInfo);

		return *this;
	}

	LuaFunctionInfo& WithReturnValue(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		return WithReturnValue(strdup(name), initialiser);
	}

	LuaFunctionInfo& WithParameterValidation()
	{
		parameterValidationEnabled = true;

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

	bool HasParameters()
	{
		return parameters.size() > 0;
	}

	const std::vector<LuaVariableInfo*>& GetReturnValues()
	{
		return returnValues;
	}

	bool HasReturnValues()
	{
		return returnValues.size() > 0;
	}


	bool ParameterValidationIsEnabled()
	{
		return parameterValidationEnabled;
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
