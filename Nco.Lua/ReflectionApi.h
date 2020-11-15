#pragma once

#include <strings.h>

#include "ILuaApi.h"
#include "ILuaRuntime.h"
#include "LuaApi.h"
#include "LuaObjectUtils.h"

class ReflectionApi : public LuaApi
{
private:
	ILuaRuntime& runtime;

	static int GetApisProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(ReflectionApi, GetApis);
	}

	ReflectionApi(ILuaRuntime& runtime) : runtime(runtime)
	{
		WithName("Reflection");

		WithDescription("Get information about API's, their functions and parameters/return values.");

		WithMethod("getApis", this, GetApisProxy, [](LuaFunctionInfo& f) {
			f.WithDescription("Get available Lua APIs")
			 .WithReturnValue("apis", [](LuaVariableInfo& r) {
				r.WithDescription("Map of APIs containing functions, parameters and return values")
				 .WithType(LuaType::Table);
			 });
		});
	}

	void WriteVariableInfo(const char* name, const std::vector<LuaVariableInfo*>& variables, ILuaStateWrapper& lua)
	{
		lua.WriteTable();

		for (auto variable : variables)
		{
			lua.WriteTable();

			lua.PushTableEntry("description", variable->GetDescription());

			if (&variable->GetType() != NULL)
			{
				lua.PushTableEntry("type", variable->GetType().value);
			}
			else
			{
				lua.PushTableEntry("type", LuaType::Any->value);
			}

			lua.SetTableIndex(variable->GetName());
		}

		lua.SetTableIndex(name);
	}

	int GetApis(ILuaStateWrapper& lua)
	{
		auto& apis = runtime.GetApis();

		lua.WriteTable();

		for (auto api : apis)
		{
			auto& functions = api->GetFunctions();

			lua.WriteTable();
			
			lua.PushTableEntry("description", api->GetDescription());

			lua.WriteTable();

			for (auto function : functions)
			{
				lua.WriteTable();

				lua.PushTableEntry("description", function->GetDescription());
				WriteVariableInfo("parameters", function->GetParameters(), lua);
				WriteVariableInfo("returnValues", function->GetReturnValues(), lua);

				lua.SetTableIndex(function->GetName());
			}

			lua.SetTableIndex("functions");

			lua.SetTableIndex(api->GetName());
		}

		return 1;
	}

public:
	static ILuaApi& Build(ILuaRuntime& runtime)
	{
		return *(new ReflectionApi(runtime));
	}

};
