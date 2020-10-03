#pragma once

#include <vector>

#include "ILuaApi.h"
#include "LuaResult.h"

typedef void (*LuaApiInitialiser)(ILuaApi&);

class ILuaRuntime
{
public:
	virtual ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser) = 0;
	virtual ILuaRuntime& RegisterApi(ILuaApi& api) = 0;
	
	virtual const std::vector<ILuaApi*>& GetApis() = 0;

	virtual LuaResult& ExecuteScript(const char* script) = 0;
	virtual LuaResult& ExecuteFile(const char* filePath) = 0;
};
