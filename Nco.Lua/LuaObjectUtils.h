#pragma once

#include <functional>

#include <lua.hpp>

#include <ResultWithValue.h>

#include "LuaStateWrapper.h"
#include "LuaParameterValidator.h"

#define LUA_METHOD_PROXY(t, m) LuaObjectUtils::BootstrapMethodCall<t>(lua, [](auto& l, auto& a) { \
	return a.m(l); \
})

class LuaObjectUtils final
{
private:
	LuaObjectUtils()
	{
	}

	template<class T> static ResultWithValue<T*>& ReadUserData(ILuaStateWrapper& lua, const char* name, int upvalueIndex)
	{
		auto objectStackIndex = lua.GetUpvalueStackIndex(upvalueIndex);
		auto& objectResult = lua.ReadUserData(objectStackIndex);

		if (objectResult.IsErrorResult())
		{
			auto& result = ResultWithValue<T*>::BuildWithError("Failed to get % s pointer : % s", name, objectResult.GetError());

			delete &objectResult;

			return result;
		}

		auto object = objectResult.GetValue();

		delete& objectResult;

		if (object == NULL)
		{
			return ResultWithValue<T*>::BuildWithError("%s pointer stored in userdata was NULL", name);
		}

		return ResultWithValue<T*>::BuildWithValue((T*)object);
	}

public:
	static ResultWithValue<LuaFunctionInfo*>& GetCurrentFunctionInfo(ILuaStateWrapper& lua)
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
		auto& functionInfoResult = GetCurrentFunctionInfo(lua);
		auto result = false;

		if (functionInfoResult.IsErrorResult())
		{
			lua.RaiseError("[Lua->C++ API] %s", functionInfoResult.GetError());
		}
		else
		{
			result = ValidateFunctionParameters(lua, *functionInfoResult.GetValue());
		}

		delete &functionInfoResult;

		return result;
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
			auto& objectResult = ReadUserData<T>(lua, "object", 2);

			if (!objectResult.IsErrorResult())
			{
				auto& object = *(objectResult.GetValue());

				result = handler(lua, object);
			}
			else
			{
				lua.RaiseError("[Lua->C++ API Object Proxy] %s", objectResult.GetError());
			}

			delete &objectResult;
		}

		delete &lua;

		return result;
	}

};
