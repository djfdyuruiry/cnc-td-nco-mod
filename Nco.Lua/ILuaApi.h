#pragma once

#include <functional>
#include <vector>

#include <lua.hpp>

#include "LuaFunctionInfo.h"

#define FunctionInitialiser std::function<void(LuaFunctionInfo&)>

class ILuaApi
{
public:
	virtual ILuaApi& WithName(const char* name) = 0;
	virtual ILuaApi& WithName(char* name) = 0;
	
	virtual ILuaApi& WithDescription(const char* description) = 0;
	virtual ILuaApi& WithDescription(char* description) = 0;

	virtual ILuaApi& WithFunction(const char* name, lua_CFunction function, FunctionInitialiser functionInitialiser) = 0;
	virtual LuaFunctionInfo& WithFunction(const char* name, lua_CFunction function) = 0;
	
	virtual LuaFunctionInfo& WithVirtualFunction(const char* name) = 0;

	virtual ILuaApi& WithMethod(const char* name, void* object, lua_CFunction methodProxy, FunctionInitialiser functionInitialiser) = 0;
	virtual LuaFunctionInfo& WithMethod(const char* name, void* object, lua_CFunction methodProxy) = 0;

	virtual const char* GetName() = 0;
	virtual const char* GetDescription() = 0;
	virtual const std::vector<LuaFunctionInfo*> GetFunctions() = 0;

};
