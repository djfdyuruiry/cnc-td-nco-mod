#pragma once

#include "ILuaStateWrapper.h"
#include "LuaResult.h"

class ILuaValueValidator
{
protected:
	ILuaValueValidator()
	{
	}

public:
	virtual LuaResult& IsValid(ILuaStateWrapper& lua, int stackIndex) = 0;

};
