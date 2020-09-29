#pragma once

#include <vector>

#include <lua.hpp>

#include "ILuaApi.h"
#include "ILuaStateWrapper.h"
#include "LuaFunctionInfo.h"

class LuaApi : public ILuaApi
{
private:
	ILuaStateWrapper& lua;

	const char* name;
	const char* description;
	std::vector<LuaFunctionInfo*>& functions;

	LuaApi(ILuaStateWrapper& lua) : lua(lua), functions(*(new std::vector<LuaFunctionInfo*>))
	{
	}

public:
	static ILuaApi& Build(ILuaStateWrapper& lua)
	{
		return *(new LuaApi(lua));
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
		auto& functionInfo = LuaFunctionInfo::Build().WithName(name);

		if (functionInitialiser != NULL)
		{
			functionInitialiser(functionInfo);
		}

		functions.push_back(&functionInfo);

		lua.WriteFunction(name, function);

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
