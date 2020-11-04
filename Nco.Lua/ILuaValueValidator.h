#pragma once

#include "ILuaStateWrapper.h"


class ILuaValueValidator
{
protected:
	ILuaValueValidator()
	{
	}

public:
	virtual bool IsValid(ILuaStateWrapper& lua, int stackIndex) = 0;

};
