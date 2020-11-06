#pragma once

#include "ILuaStateWrapper.h"
#include "LuaApi.h"
#include "LuaObjectUtils.h"

class RuleValueApi : public LuaApi
{
protected:
	static int ReadRuleValueApiProxy(lua_State* lua)
	{
		return LuaObjectUtils::BootstrapProxyCall<RuleValueApi>(lua, [](ILuaStateWrapper& l, RuleValueApi& a) {
			return a.ReadRuleLua(l);
		});
	}

	static int WriteRuleValueApiProxy(lua_State* lua)
	{
		return LuaObjectUtils::BootstrapProxyCall<RuleValueApi>(lua, [](ILuaStateWrapper& l, RuleValueApi& a) {
			return a.WriteRuleLua(l);
		});
	}

	RuleValueApi()
	{
	}

public:
	virtual int ReadRuleLua(ILuaStateWrapper& lua) = 0;

	virtual int WriteRuleLua(ILuaStateWrapper& lua) = 0;

};
