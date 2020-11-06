#include <functional>

#include <lua.hpp>

int LambdaProxy(lua_State* lua)
{
	auto lambdaUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto lambda = (const std::function<int(lua_State*)>*) lambdaUserData;

	return (*lambda)(lua);
}
