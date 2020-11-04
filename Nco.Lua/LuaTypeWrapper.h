#pragma once

#include <functional>
#include <map>

#include "ILuaStateWrapper.h"

class LuaTypeWrapper
{
private:
	std::map<int, std::function<void(ILuaStateWrapper&)>>& extractors;
	std::map<int, std::function<void(ILuaStateWrapper&)>>& injectors;

	LuaTypeWrapper(): 
		extractors(*(new std::map<int, std::function<void(ILuaStateWrapper&)>>())),
		injectors(*(new std::map<int, std::function<void(ILuaStateWrapper&)>>))
	{

	}
};
