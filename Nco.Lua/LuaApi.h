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
	LuaApi() : functions(*(new std::vector<LuaFunctionInfo*>))
	{
	}

public:
	static ILuaApi& Build()
	{
		return *(new LuaApi());
	}

	~LuaApi()
	{
		delete name;
		delete description;

		for (auto function : functions) {
			delete function;
		}

		delete &functions;
	}

	ILuaApi& WithName(const char* name)
	{
		this->name = name;

		return *this;
	}

	ILuaApi& WithDescription(const char* description)
	{
		this->description = description;

		return *this;
	}

	ILuaApi& WithFunction(const char* name, lua_CFunction function, FunctionInitialiser functionInitialiser)
	{
		auto& functionInfo = LuaFunctionInfo::Build()
			.WithName(name)
			.WithImplementation(function);

		if (functionInitialiser != NULL)
		{
			functionInitialiser(functionInfo);
		}

		functions.push_back(&functionInfo);

		return *this;
	}

	ILuaApi& WithMethod(const char* name, void* object, lua_CFunction methodProxy, FunctionInitialiser functionInitialiser)
	{
		auto& functionInfo = LuaFunctionInfo::Build()
			.WithName(name)
			.WithImplementation(object, methodProxy);

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
