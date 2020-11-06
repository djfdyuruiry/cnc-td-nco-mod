#pragma once

#include <lua.hpp>

#include "LuaObjectUtils.h"
#include "RuleValueApi.h"

int ReadRuleValueApiProxy(lua_State* lua)
{
	return LuaObjectUtils::BootstrapProxyCall<RuleValueApi>(lua, [](ILuaStateWrapper& l, RuleValueApi& a) {
		return a.ReadRuleLua(l);
	});
}

int WriteRuleValueApiProxy(lua_State* lua)
{
	return LuaObjectUtils::BootstrapProxyCall<RuleValueApi>(lua, [](ILuaStateWrapper& l, RuleValueApi& a) {
		return a.WriteRuleLua(l);
	});
}
