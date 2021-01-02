#pragma once

#include <functional>
#include <vector>

#include "ILuaApi.h"
#include "ILuaStateWrapper.h"
#include <Result.h>

#define LuaApiInitialiser std::function<void(ILuaApi&)>

class ILuaRuntime
{
public:
	virtual ILuaRuntime& RegisterApi(const char* name, LuaApiInitialiser initialiser) = 0;
	virtual ILuaRuntime& RegisterApi(ILuaApi& api) = 0;
	
	virtual const std::vector<ILuaApi*>& GetApis() = 0;

	virtual Result& ExecuteScript(const char* script) = 0;
	virtual Result& ExecuteFile(const char* filePath) = 0;

	virtual ILuaStateWrapper& GetState() = 0;
};
