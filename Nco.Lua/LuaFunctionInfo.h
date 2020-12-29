#pragma once

#include <functional>
#include <vector>

#include <lua.hpp>

#include "LuaVariableInfo.h"

#define LuaVariableInfoInitialiser std::function<void(LuaVariableInfo&)>

class LuaFunctionInfo
{
private:
	char* name;
	char* description;
	bool parameterValidationEnabled;

	lua_CFunction luaFunction;

	bool isVirtual;
	void* implementationObject;
	lua_CFunction methodProxy;

	std::vector<LuaVariableInfo*>& parameters;
	std::vector<LuaVariableInfo*>& returnValues;

	unsigned int requiredParameterCount;

	LuaFunctionInfo(bool isVirtual = false) : parameters(*(new std::vector<LuaVariableInfo*>())),
		returnValues(*(new std::vector<LuaVariableInfo*>())),
		isVirtual(isVirtual),
		implementationObject(NULL),
		parameterValidationEnabled(false),
		requiredParameterCount(0)
	{
	}

public:
	static LuaFunctionInfo& Build()
	{
		return *(new LuaFunctionInfo());
	}

	static LuaFunctionInfo& BuildVirutal()
	{
		return *(new LuaFunctionInfo(true));
	}

	~LuaFunctionInfo()
	{
		if (name != NULL)
		{
			delete name;
		}

		if (description != NULL)
		{
			delete description;
		}

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
		luaFunction = impl;

		isVirtual = false;

		return *this;
	}

	LuaFunctionInfo& WithImplementation(void* object, lua_CFunction objectMethodProxy)
	{
		implementationObject = object;
		methodProxy = objectMethodProxy;

		isVirtual = false;

		return *this;
	}

	LuaFunctionInfo& WithDescription(char* description)
	{
		this->description = description;

		return *this;
	}

	LuaFunctionInfo& WithDescription(const char* description)
	{
		return WithDescription(strdup(description));
	}

	LuaVariableInfo& WithParameter(char* name, LuaVariableInfoInitialiser initialiser, bool isOptional = false)
	{
		auto& varInfo = LuaVariableInfo::BuildParameter(parameters.size() + 1)
			.WithName(name);

		if (initialiser != NULL)
		{
			initialiser(varInfo);
		}

		parameters.push_back(&varInfo);

		if (!isOptional)
		{
			requiredParameterCount++;
		}

		return varInfo;
	}

	LuaVariableInfo& WithParameter(const char* name)
	{
		return WithParameter(strdup(name), NULL);
	}

	LuaFunctionInfo& WithParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		WithParameter(strdup(name), initialiser);

		return *this;
	}

	LuaVariableInfo& WithOptionalParameter(const char* name)
	{
		return WithParameter(strdup(name), NULL, true);
	}

	LuaFunctionInfo& WithOptionalParameter(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		WithParameter(strdup(name), initialiser, true);

		return *this;
	}

	LuaVariableInfo& WithReturnValue(const char* name)
	{
		auto& varInfo = LuaVariableInfo::BuildReturnValue(returnValues.size() + 1)
			.WithName(name);

		returnValues.push_back(&varInfo);

		return varInfo;
	}

	LuaFunctionInfo& WithReturnValue(const char* name, LuaVariableInfoInitialiser initialiser)
	{
		auto& varInfo = WithReturnValue(name);

		if (initialiser != NULL)
		{
			initialiser(varInfo);
		}

		return *this;
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

	bool IsVirtual()
	{
		return isVirtual;
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

	unsigned int GetRequiredParameterCount()
	{
		return requiredParameterCount;
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
		if (luaFunction == NULL || lua == NULL)
		{
			return 0;
		}

		return luaFunction(lua);
	}

};
