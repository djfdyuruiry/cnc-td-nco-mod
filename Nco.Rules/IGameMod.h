#pragma once

#include <HashUtils.h>

class IGameMod
{
protected:
	IGameMod()
	{
	}

public:
	virtual void ReadTypes() = 0;

	virtual void InitialiseTypes() = 0;

	virtual StringHash GetTypeKey() = 0;

	virtual const char* GetTypeName() = 0;

};
