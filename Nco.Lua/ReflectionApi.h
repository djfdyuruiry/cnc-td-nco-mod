#pragma once

#include <functional>

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

	static int RegisterApiLuaProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(ReflectionApi, RegisterApiLua);
	}

	static int RegisterApiFunctionsLuaProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(ReflectionApi, RegisterApiFunctionsLua);
	}

	ReflectionApi(ILuaRuntime& runtime) : runtime(runtime)
	{
		WithName("Reflection");

		WithDescription("Get information about API's, their functions and parameters/return values.");

		WithMethod("getApis", this, GetApisProxy, [](LuaFunctionInfo& f) {
			f.WithDescription("Get available Lua APIs")
			 .WithReturnValue("apis", [](LuaVariableInfo& r) {
				r.WithDescription("Map of APIs containing info on APIs and their functions, along with function parameters and return values")
				 .WithType(LuaType::Table);
			 });
		});

		WithMethod("registerApi", this, RegisterApiLuaProxy, [](LuaFunctionInfo& f) {
			f.WithDescription("Add a new API to the reflection registry")
			 .WithParameter("api", [](LuaVariableInfo& r) {
				r.WithDescription("Table containing: \n\
\n\
  'name' - API name \n\
  'description' - API description \n\
  'functions' - dictionary containing API functions, each item should be a table containing: \n\
	'description' - function description \n\
    'parameters' - dictionary containing function parameters, each item should be a table containing: \n\
      'description' - parameter description \n\
      'type' - parameter lua type (table, number etc.) \n\
    'returnValues' - dictionary containing function return value(s), each item should be a table containing: \n\
      'description' - parameter description \n\
      'type' - parameter lua type (table, number etc.)")
				 .WithType(LuaType::Table);
			 });
		});

		WithMethod("registerApiFunctions", this, RegisterApiFunctionsLuaProxy, [](LuaFunctionInfo& f) {
			f.WithDescription("Add additional methods to an existing API in the reflection registry")
			 .WithParameter("apiName", [](LuaVariableInfo& r) {
				r.WithDescription("Name of the API to add new methods to, case sensitive")
			     .WithType(LuaType::String);
			 })
			 .WithParameter("functions", [](LuaVariableInfo& r) {
				r.WithDescription("Dictionary containing API functions, each item should be a table containing: \n\
	'description' - function description \n\
    'parameters' - dictionary containing function parameters, each item should be a table containing: \n\
      'description' - parameter description \n\
      'type' - parameter lua type (table, number etc.) \n\
    'returnValues' - dictionary containing function return value(s), each item should be a table containing: \n\
      'description' - parameter description \n\
      'type' - parameter lua type (table, number etc.)")
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

	void ReadApiVariableInfo(
		ILuaStateWrapper& lua,
		LuaVariableInfo& variable,
		int stackIndex
	)
	{
		auto& paramResult = lua.IterateOverTable<const char*>(stackIndex, [&](auto variableKey, auto variableValueIndex, auto& _)
		{
			if (StringsAreEqual(variableKey, "description"))
			{
				lua.PullOptionalValue<const char*>(variableValueIndex, [&](auto description)
				{
					variable.WithDescription(description);
				});
			}
			else if (StringsAreEqual(variableKey, "type"))
			{
				lua.PullOptionalValue<const char*>(variableValueIndex, [&](auto luaType)
				{
					variable.WithType(LuaType::Parse(luaType));
				});
			}
		});

		delete& paramResult;
	}

	void ReadApiFunctionInfo(
		ILuaStateWrapper& lua,
		LuaFunctionInfo& apiFunction,
		int stackIndex
	)
	{
		auto& functionResult = lua.IterateOverTable<const char*>(stackIndex, [&](auto functionKey, auto functionValueIndex, auto& _)
		{
			if (StringsAreEqual(functionKey, "description"))
			{
				lua.PullOptionalValue<const char*>(functionValueIndex, [&](auto description)
				{
					apiFunction.WithDescription(description);
				});
			}
			else if (StringsAreEqual(functionKey, "parameters"))
			{
				auto& paramsResult = lua.IterateOverTable<const char*>(functionValueIndex, [&](auto paramsKey, auto paramsValueIndex, auto& _)
				{
					auto& param = apiFunction.WithParameter(paramsKey);
							
					ReadApiVariableInfo(lua, param, paramsValueIndex);
				});

				delete &paramsResult;
			}
			else if (StringsAreEqual(functionKey, "returnValues"))
			{
				auto& returnValuesResult = lua.IterateOverTable<const char*>(functionValueIndex, [&](auto returnValuesKey, auto returnValuesValueIndex, auto& _)
				{
					auto& returnValue = apiFunction.WithReturnValue(returnValuesKey);

					ReadApiVariableInfo(lua, returnValue, returnValuesValueIndex);
				});

				delete &returnValuesResult;
			}
		});

		delete &functionResult;
	}

	void ReadApiFunctions(ILuaStateWrapper& lua, ILuaApi& api, int stackIndex)
	{
		auto& functionsResult = lua.IterateOverTable<const char*>(stackIndex, [&](auto functionsKey, auto functionsValueIndex, auto& _)
		{
			auto& apiFunction = api.WithVirtualFunction(functionsKey);

			ReadApiFunctionInfo(lua, apiFunction, functionsValueIndex);
		});

		delete &functionsResult;
	}

	int RegisterApiLua(ILuaStateWrapper& lua)
	{
		auto& newApi = LuaApi::Build();
		auto isValid = false;

		auto& apiTableResult = lua.IterateOverTable<const char*>([&](auto apiKey, auto apiValueIndex, auto& _)
		{
			if (StringsAreEqual(apiKey, "name"))
			{
				auto& nameResult = lua.ReadString(apiValueIndex);
				
				if (!nameResult.IsErrorResult())
				{
					newApi.WithName(nameResult.GetValue());

					isValid = true;
				}

				delete &nameResult;
			}
			else if (StringsAreEqual(apiKey, "description"))
			{
				lua.PullOptionalValue<const char*>(apiValueIndex, [&](auto description)
				{
					newApi.WithDescription(description);
				});
			}
			else if (StringsAreEqual(apiKey, "functions"))
			{
				ReadApiFunctions(lua, newApi, apiValueIndex);
			}
		});

		if (!apiTableResult.IsErrorResult() && isValid)
		{
			runtime.RegisterApi(newApi);
		}
		else
		{
			lua.RaiseError("Parameter 'api' must be a table with at least one field, 'name' - which should be a string.");
		}

		delete &apiTableResult;

		return 0;
	}

	int RegisterApiFunctionsLua(ILuaStateWrapper& lua)
	{
		auto& apiNameResult = lua.ReadString(1);

		auto apiFound = false;
		auto apiName = apiNameResult.GetValue();

		delete &apiNameResult;

		for (auto api : runtime.GetApis())
		{
			if (StringsAreEqual(apiName, api->GetName()))
			{
				apiFound = true;

				ReadApiFunctions(lua, *api, 2);

				break;
			}
		}

		if (!apiFound)
		{
			lua.RaiseError("No API is currently registered with the given name: %s", apiName);
		}

		return 0;
	}

public:
	static ILuaApi& Build(ILuaRuntime& runtime)
	{
		return *(new ReflectionApi(runtime));
	}

};
