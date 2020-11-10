#pragma once

#include <vector>

#include <Logger.h>
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

	~LuaRuntime()
	{
		for (auto api : apis) {
			delete api;
		}

		delete &apis;

		delete &lua;
	}

	ILuaRuntime& RegisterApi(ILuaApi& api)
	{
		for (auto function : api.GetFunctions())
		{
			if (!function->IsObjectMethod())
			{
				lua.WriteFunction(function->GetName(), function->GetFunction());
			}
			else
			{
				lua.WriteMethod(function->GetName(), function->GetImplementationObject(), function->GetMethodProxy());
			}
		}

		apis.push_back(&api);

		return *this;
	}

	ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser)
	{
		Log_Debug("Registering Lua API: %s", name);

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

	ILuaStateWrapper& GetState()
	{
		return lua;
	}

};
