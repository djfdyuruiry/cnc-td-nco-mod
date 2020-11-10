#pragma once

#include "ILuaStateWrapper.h"
#include "ILuaValueValidator.h"

class LuaValueAdapter
{
private:
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
		return lua.PullValue<T>(stackIndex).GetValue();
	}

	template<class T> T Read(ILuaStateWrapper& lua)
	{
		return lua.PullValue<T>(lua.GetStackTop()).GetValue();
	}

	template<class T> void Write(ILuaStateWrapper& lua, T value)
	{
		lua.PushValue<T>(value);
	}

};
