#pragma once

#include "ILuaStateWrapper.h"
#include "LuaApi.h"

int ReadRuleValueApiProxy(lua_State* lua);

int WriteRuleValueApiProxy(lua_State* lua);

class RuleValueApi : public LuaApi
{
protected:
	RuleValueApi()
	{
	}

public:
	virtual int ReadRuleLua(ILuaStateWrapper& lua) = 0;

	virtual int WriteRuleLua(ILuaStateWrapper& lua) = 0;

};
