#pragma once

#include <lua.hpp>

#include "RuleValueApi.h"

int ReadRuleValueApiProxy(lua_State* lua)
{
	auto instancePtrUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto instancePtr = (RuleValueApi*)instancePtrUserData;

	return instancePtr->ReadRuleLua(lua);
}

int WriteRuleValueApiProxy(lua_State* lua)
{
	auto instancePtrUserData = lua_touserdata(lua, lua_upvalueindex(1));
	auto instancePtr = (RuleValueApi*)instancePtrUserData;

	return instancePtr->WriteRuleLua(lua);
}

