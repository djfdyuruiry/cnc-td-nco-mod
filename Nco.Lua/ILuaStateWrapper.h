#pragma once

#include <lua.hpp>

class ILuaStateWrapper
{
public:
	virtual void ExecuteScript(const char* script) = 0;

	virtual void ExecuteFile(const char* filePath) = 0;

	virtual int ReadInteger(unsigned int stackIndex) = 0;

	virtual double ReadDouble(unsigned int stackIndex) = 0;

	virtual const char* ReadString(unsigned int stackIndex) = 0;

	virtual void WriteInteger(int value) = 0;

	virtual void WriteDouble(double value) = 0;

	virtual void WriteString(const char* value) = 0;
};
