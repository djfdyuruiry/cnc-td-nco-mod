#pragma once

#include <functional>

#include <lua.hpp>

#include "LuaStateWrapper.h"

class LuaObjectUtils final
{
private:
	LuaObjectUtils()
	{
	}

public:
	template<class T> static int BootstrapProxyCall(lua_State* lua, std::function<int(ILuaStateWrapper&, T&)> handler)
	{
		auto& state = LuaStateWrapper::Build(lua, false);

		auto stackIndex = state.GetUpvalueStackIndex(1);
		auto& objectResult = state.ReadUserData(stackIndex);

		if (objectResult.IsErrorResult())
		{
			state.RaiseError("lua->c++ object proxy error: %s", objectResult.GetError());

			delete &objectResult;
			delete &state;

			return 0;
		}

		auto object = objectResult.GetValue();

		delete &objectResult;

		if (object == NULL)
		{
			state.RaiseError("lua->c++ object proxy error: Object pointer stored in userdata was NULL");
			
			delete &state;

			return 0;
		}

		auto& objectRef = *((T*)object);

		auto result = handler(state, objectRef);

		delete &state;

		return result;
	}
};
