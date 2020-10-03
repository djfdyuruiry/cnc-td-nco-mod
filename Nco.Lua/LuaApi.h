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

	LuaApi() : functions(*(new std::vector<LuaFunctionInfo*>))
	{
	}

public:
	static ILuaApi& Build()
	{
		return *(new LuaApi());
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
