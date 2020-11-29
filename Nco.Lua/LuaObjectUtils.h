#pragma once

#include <functional>

#include <lua.hpp>

#include "LuaStateWrapper.h"
#include "LuaParameterValidator.h"

#define LUA_METHOD_PROXY(t, m) LuaObjectUtils::BootstrapProxyCall<t>(lua, [](ILuaStateWrapper& l, t& a) { \
	return a.m(l); \
})

class LuaObjectUtils final
{
private:
	LuaObjectUtils()
	{
	}

	template<class T> static T* ReadUserData(ILuaStateWrapper& state, const char* name, int upvalueIndex)
	{
		auto objectStackIndex = state.GetUpvalueStackIndex(1);
		auto& objectResult = state.ReadUserData(objectStackIndex);

		if (objectResult.IsErrorResult())
		{
			state.RaiseError("[lua->c++ object proxy] Failed to get %s pointer: %s", name, objectResult.GetError());

			delete &objectResult;
			delete &state;

			return 0;
		}

		auto object = objectResult.GetValue();

		delete &objectResult;

		if (object == NULL)
		{
			state.RaiseError("[lua->c++ object proxy error] %s pointer stored in userdata was NULL", name);

			delete &state;

			return NULL;
		}

		return (T*)object;
	}

public:
	template<class T> static int BootstrapProxyCall(
		lua_State* lua,
		std::function<int(ILuaStateWrapper&, T&)> handler
	)
	{
		auto& state = LuaStateWrapper::Build(lua, false);

		auto object = ReadUserData<T>(state, "object", 1);
		auto functionInfo = ReadUserData<LuaFunctionInfo>(state, "function info", 2);

		if (object == NULL || functionInfo == NULL)
		{
			return 0;
		}

		if
		(
			functionInfo->ParameterValidationIsEnabled()
			&& !LuaParameterValidator::ValidateInvocationParameters(*functionInfo, state)
		)
		{
			return 0;
		}

		auto result = handler(state, *object);

		delete &state;

		return result;
	}

};
