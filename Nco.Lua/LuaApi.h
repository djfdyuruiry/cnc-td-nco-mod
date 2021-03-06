#pragma once

#include <vector>

#include <lua.hpp>

#include "ILuaApi.h"
#include "LuaFunctionInfo.h"

class LuaApi : public ILuaApi
{
private:
	const char* name;
	const char* description;
	std::vector<LuaFunctionInfo*>& functions;

protected:
	LuaApi() : functions(*(new std::vector<LuaFunctionInfo*>)), name(NULL), description(NULL)
	{
	}

public:
	static ILuaApi& Build()
	{
		return *(new LuaApi());
	}

	~LuaApi()
	{
		if (name != NULL)
		{
			delete name;
		}

		if (description != NULL)
		{
			delete description;
		}

		for (auto function : functions) {
			delete function;
		}

		delete &functions;
	}

	ILuaApi& WithName(const char* name)
	{
		return WithName(strdup(name));
	}

	ILuaApi& WithName(char* name)
	{
		this->name = name;

		return *this;
	}

	ILuaApi& WithDescription(const char* description)
	{
		return WithDescription(strdup(description));
	}

	ILuaApi& WithDescription(char* description)
	{
		this->description = description;

		return *this;
	}

	ILuaApi& WithFunction(const char* name, lua_CFunction function, FunctionInitialiser functionInitialiser)
	{
		auto& functionInfo = LuaFunctionInfo::Build()
			.WithName(name)
			.WithImplementation(function)
			.WithParameterValidation();

		if (functionInitialiser != NULL)
		{
			functionInitialiser(functionInfo);
		}

		functions.push_back(&functionInfo);

		return *this;
	}

	LuaFunctionInfo& WithFunction(const char* name, lua_CFunction function)
	{
		WithFunction(name, function, NULL);

		return *functions.back();
	}

	LuaFunctionInfo& WithVirtualFunction(const char* name)
	{
		auto& functionInfo = LuaFunctionInfo::BuildVirutal()
			.WithName(name);

		functions.push_back(&functionInfo);

		return functionInfo;
	}

	ILuaApi& WithMethod(const char* name, void* object, lua_CFunction methodProxy, FunctionInitialiser functionInitialiser)
	{
		auto& functionInfo = LuaFunctionInfo::Build()
			.WithName(name)
			.WithImplementation(object, methodProxy)
			.WithParameterValidation();

		if (functionInitialiser != NULL)
		{
			functionInitialiser(functionInfo);
		}

		functions.push_back(&functionInfo);

		return *this;
	}

	LuaFunctionInfo& WithMethod(const char* name, void* object, lua_CFunction methodProxy)
	{
		WithMethod(name, object, methodProxy, NULL);

		return *functions.back();
	}

	const char* GetName()
	{
		return name;
	}

	const char* GetDescription()
	{
		return description;
	}

	const std::vector<LuaFunctionInfo*> GetFunctions()
	{
		return functions;
	}

};
