#pragma once

#include <vector>

#include "ILuaApi.h"

typedef void (*LuaApiInitialiser)(ILuaApi&);

class ILuaRuntime
{
public:
	virtual ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser) = 0;
	virtual const std::vector<ILuaApi*>& GetApis() = 0;
};
