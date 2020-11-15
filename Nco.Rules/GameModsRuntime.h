#pragma once

#include <map>

#include <logger.h>
#include <HashUtils.h>

#include "IGameMod.h"
#include "IRulesRuntime.h"
#include "NcoGameMod.h"

class GameModsRuntime
{
private:
	bool typesRead;
	IRulesRuntime& rulesRuntime;
	std::map<StringHash, IGameMod*>& mods;

	GameModsRuntime(IRulesRuntime& rulesRuntime) :
		rulesRuntime(rulesRuntime),
		mods(*(new std::map<StringHash, IGameMod*>())),
		typesRead(false)
	{
	}

public:
	static GameModsRuntime& Build(IRulesRuntime& rulesRuntime)
	{
		return *(new GameModsRuntime(rulesRuntime));
	}

	~GameModsRuntime()
	{
		for (auto [_, mod] : mods)
		{
			delete mod;
		}

		delete &mods;
	}

	void ReadTypesIfRequired()
	{
		if (typesRead)
		{
			return;
		}

		typesRead = true;

		Log_Info("Reading NCO mod types");

		for (auto [_, mod] : mods)
		{
			mod->ReadTypes();
		}
	}

	bool InitaliseTypes()
	{
		Log_Info("Initialising NCO mod types");

		auto initSuccess = true;

		for (auto [_, mod] : mods)
		{
			if (!mod->InitialiseTypes())
			{
				initSuccess = false;
			}
		}

		return initSuccess;
	}

	template<class T> GameModsRuntime& RegisterMod()
	{
		auto& mod = T::Build(rulesRuntime);

		mods[mod.GetTypeKey()] = (IGameMod*)&mod;

		Log_Info("NCO mod registered: %s", mod.GetTypeName());

		return *this;
	}

	template<class T, class U> NcoGameMod<T, U>& GetModByTypeKey(StringHash typeKey)
	{
		return (NcoGameMod<T, U>&)*mods[typeKey];
	}

};
