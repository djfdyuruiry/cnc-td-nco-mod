#pragma once

#include <functional>

#include <lua.hpp>

#include "LuaStateWrapper.h"
#include "LuaParameterValidator.h"

#define LUA_METHOD_PROXY(t, m) LuaObjectUtils::BootstrapMethodCall<t>(lua, [](ILuaStateWrapper& l, t& a) { \
	return a.m(l); \
})

class LuaObjectUtils final
{
private:
	LuaObjectUtils()
	{
	}

	template<class T> static T* ReadUserData(ILuaStateWrapper& lua, const char* name, int upvalueIndex)
	{
		auto objectStackIndex = lua.GetUpvalueStackIndex(upvalueIndex);
		auto& objectResult = lua.ReadUserData(objectStackIndex);

		if (objectResult.IsErrorResult())
		{
			lua.RaiseError("[lua->c++ object proxy] Failed to get %s pointer: %s", name, objectResult.GetError());

			delete &objectResult;

			return 0;
		}

		auto object = objectResult.GetValue();

		delete& objectResult;

		if (object == NULL)
		{
			lua.RaiseError("[lua->c++ object proxy error] %s pointer stored in userdata was NULL", name);

			delete &lua;

			return NULL;
		}

		return (T*)object;
	}

public:
	static LuaFunctionInfo* GetCurrentFunctionInfo(ILuaStateWrapper& lua)
	{
		return ReadUserData<LuaFunctionInfo>(lua, "function info", 1);
	}

	static bool ValidateFunctionParameters(ILuaStateWrapper& lua, LuaFunctionInfo& functionInfo)
	{
		return !functionInfo.ParameterValidationIsEnabled()
			|| LuaParameterValidator::ValidateInvocationParameters(functionInfo, lua);
	}

	static bool ValidateCurrentFunctionParameters(ILuaStateWrapper& lua)
	{
		auto functionInfo = GetCurrentFunctionInfo(lua);

		return functionInfo != NULL
			&& ValidateFunctionParameters(lua, *functionInfo);
	}

	template<class T> static int BootstrapMethodCall(
		lua_State* luaState,
		std::function<int(ILuaStateWrapper&, T&)> handler
	)
	{
		auto& lua = LuaStateWrapper::Build(luaState, false);
		auto result = 0;

		if (ValidateCurrentFunctionParameters(lua))
		{
			auto object = ReadUserData<T>(lua, "object", 2);

			if (object != NULL)
			{
				result = handler(lua, *object);
			}
		}

		delete &lua;

		return result;
	}

};
