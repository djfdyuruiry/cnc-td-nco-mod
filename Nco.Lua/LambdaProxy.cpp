#include <functional>

#include <lua.hpp>

#include "LuaMethod.h"

int LambdaProxy(lua_State* lua)
{
	auto lambdaFactoryUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto instancePtrUserData = lua_touserdata(lua, lua_upvalueindex(2));
	
	auto lambdaFactory = (std::function<LuaMethod(void*)>*) lambdaFactoryUserData;
	auto lambda = lambdaFactory->operator()(instancePtrUserData);

	return lambda(lua);
}
