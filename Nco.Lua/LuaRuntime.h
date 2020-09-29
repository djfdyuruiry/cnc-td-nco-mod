#pragma once

#include <vector>

#include "ILuaRuntime.h"
#include "LuaApi.h"

class LuaRuntime : public ILuaRuntime
{
private:
	ILuaStateWrapper& lua;
	std::vector<ILuaApi*>& apis;

	LuaRuntime(ILuaStateWrapper& lua) : lua(lua), apis(*(new std::vector<ILuaApi*>()))
	{
	}

public:
	static ILuaRuntime& Build(ILuaStateWrapper& lua)
	{
		return *(new LuaRuntime(lua));
	}

	ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser)
	{
		auto& api = LuaApi::Build(lua).WithName(name);

		if (initialiser != NULL)
		{
			initialiser(api);
		}

		apis.push_back(&api);

		return *this;
	}

	const std::vector<ILuaApi*>& GetApis()
	{
		return apis;
	};
};
