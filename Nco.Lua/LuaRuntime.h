#pragma once

#include <vector>

#include <FileUtils.h>
#include <strings.h>

#include "ILuaRuntime.h"
#include "ILuaStateWrapper.h"
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

	ILuaRuntime& RegisterApi(ILuaApi& api)
	{
		for (auto function : api.GetFunctions())
		{
			lua.WriteFunction(function->GetName(), function->GetLuaFunction());
		}

		apis.push_back(&api);

		return *this;
	}

	ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser)
	{
		auto& api = LuaApi::Build().WithName(name);

		if (initialiser != NULL)
		{
			initialiser(api);
		}

		return RegisterApi(api);
	}

	const std::vector<ILuaApi*>& GetApis()
	{
		return apis;
	};

	LuaResult& ExecuteScript(const char* script)
	{
		if (!FileUtils::IsFile(script))
		{
			return LuaResult::Build(
				FormatString("Lua script file was not found: '%s'", script)
			);
		}

		return lua.ExecuteScript(script);
	}

	LuaResult& ExecuteFile(const char* filePath)
	{
		return lua.ExecuteFile(filePath);
	}
};
