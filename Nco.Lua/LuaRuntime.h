#pragma once

#include <vector>

#include <Logger.h>
#include <FileUtils.h>
#include <strings.h>

#include "ILuaRuntime.h"
#include "ILuaStateWrapper.h"
#include "LuaApi.h"
#include "LuaObjectUtils.h"
#include "ReflectionApi.h"

class LuaRuntime : public ILuaRuntime
{
private:
	ILuaStateWrapper& lua;
	std::vector<ILuaApi*>& apis;

	LuaRuntime(ILuaStateWrapper& lua) : lua(lua), apis(*(new std::vector<ILuaApi*>()))
	{
		LuaType::InitIfRequired();

		RegisterApi(ReflectionApi::Build(*this));
	}

public:
	static ILuaRuntime& Build(ILuaStateWrapper& lua)
	{
		return *(new LuaRuntime(lua));
	}

	~LuaRuntime()
	{
		delete &lua;

		for (auto api : apis) {
			delete api;
		}

		delete &apis;
	}

	ILuaRuntime& RegisterApi(ILuaApi& api)
	{
		auto& ncoTableResult = lua.PushGlobalOntoStack("Nco");

		if (ncoTableResult.IsErrorResult())
		{
			ShowError("Failed to get Nco global table from lua state: %s", ncoTableResult.GetError());

			delete &ncoTableResult;

			return *this;
		}

		delete &ncoTableResult;

		lua.WriteTable();

		for (auto function : api.GetFunctions())
		{
			if (!function->IsObjectMethod())
			{
				lua.WriteFunction(function->GetFunction(), function);
			}
			else
			{
				lua.WriteMethod(function->GetImplementationObject(), function->GetMethodProxy(), function);
			}

			lua.SetTableIndex(function->GetName());
		}

		lua.SetTableIndex(api.GetName());

		apis.push_back(&api);

		return *this;
	}

	ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser)
	{
		LogDebug("Registering Lua API: %s", name);

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

	Result& ExecuteScript(const char* script)
	{
		return lua.ExecuteScript(script);
	}

	Result& ExecuteFile(const char* filePath)
	{
		if (!FileUtils::IsFile(filePath))
		{
			return Result::BuildWithError(
				"Lua script file was not found: '%s'",
				filePath
			);
		}

		return lua.ExecuteFile(filePath);
	}

	ILuaStateWrapper& GetState()
	{
		return lua;
	}

};
