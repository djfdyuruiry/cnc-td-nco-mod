#pragma once

#include <Result.h>

#include "ILuaStateWrapper.h"

class ILuaValueValidator
{
protected:
	ILuaValueValidator()
	{
	}

public:
	virtual Result& IsValid(ILuaStateWrapper& lua, int stackIndex) = 0;

};
