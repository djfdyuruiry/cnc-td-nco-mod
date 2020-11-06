#pragma once

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
	virtual int ReadRuleLua(lua_State* lua) = 0;

	virtual int WriteRuleLua(lua_State* lua) = 0;

};
