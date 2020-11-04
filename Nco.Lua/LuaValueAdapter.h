#pragma once

#include "ILuaStateWrapper.h"
#include "ILuaValueValidator.h"

class LuaValueAdapter
{
protected:
	ILuaValueValidator& validator;

	LuaValueAdapter(ILuaValueValidator& validator) : validator(validator)
	{
	}

public:
	static LuaValueAdapter& Build(ILuaValueValidator& validator)
	{
		return *(new LuaValueAdapter(validator));
	}

	template<class T> T Read(ILuaStateWrapper& lua, int stackIndex)
	{
		return lua.PullValue(stackIndex);
	}

	template<class T> T Read(ILuaStateWrapper& lua)
	{
		return lua.PullValue(lua.GetStackTop());
	}

	template<class T> void Write(ILuaStateWrapper& lua, T value)
	{
		return lua.PushValue(value);
	}
};
