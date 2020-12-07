#pragma once

#include "ILuaStateWrapper.h"
#include <Result.h>

class ILuaValueValidator
{
protected:
	ILuaValueValidator()
	{
	}

public:
	virtual Result& IsValid(ILuaStateWrapper& lua, int stackIndex) = 0;

};
