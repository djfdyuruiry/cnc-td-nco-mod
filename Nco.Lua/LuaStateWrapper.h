#pragma once

#include "ILuaStateWrapper.h"

class LuaStateWrapper : public ILuaStateWrapper
{
private:
	lua_State* lua;

	LuaStateWrapper(lua_State* lua)
	{
		this->lua = lua;
	}

public:
	static LuaStateWrapper& Build(lua_State* lua)
	{
		return *(new LuaStateWrapper(lua));
	}

};
