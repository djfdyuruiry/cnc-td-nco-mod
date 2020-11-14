#pragma once

#include <stdlib.h>

#include <ILuaRuntime.h>
#include <LuaRuntime.h>
#include <LuaStateFactory.h>
#include <LuaStateWrapper.h>
#include <Utils.h>

#include "EnhancementKeys.h"
#include "GameModsRuntime.h"
#include "IRulesRuntime.h"
#include "RulesRuntime.h"

class NcoRuntime
{
private:
	void InitialiseMods()
	{
		RegisterMods();

		modRuntime.ReadTypes();
	}

	bool LoadLuaScripts()
	{
		auto executionOk = true;
		auto& scripts = rulesRuntime.GetLuaScripts();

		for (auto scriptFile : scripts) {
			auto scriptFilePath = Build_Mod_Data_File_Path(scriptFile);
			auto& executeResult = luaRuntime.ExecuteFile(scriptFilePath);

			if (executeResult.IsErrorResult())
			{
				Log_Error("Error loading rules lua script '%s': %s", scriptFilePath, executeResult.GetError());

				delete& executeResult;

				executionOk = false;
			}
		}

		return executionOk;
	}

	bool LoadLuaComponents()
	{
		if (!rulesRuntime.LuaIsEnabled())
		{
			return true;
		}

		if (!InitialiseLuaApi())
		{
			return false;
		}

		Log_Debug("Lua API intialised");

		if (!InitialiseLuaEvents())
		{
			return false;
		}

		Log_Debug("Lua events system initialised");

		if (!LoadLuaScripts())
		{
			return false;
		}

		Log_Debug("Lua scripts from Rules file loaded");

		return true;
	}

protected:
	bool rulesInitSuccessful;
	bool luaInitSuccessful;
	bool modsInitialised;
	IRulesRuntime& rulesRuntime;
	ILuaRuntime& luaRuntime;
	GameModsRuntime& modRuntime;

	NcoRuntime(IRulesRuntime& rulesRuntime) :
		rulesRuntime(rulesRuntime),
		luaRuntime(
			LuaRuntime::Build(
				LuaStateWrapper::Build(
					LuaStateFactory::Build(Get_Mod_Data_Path())
				)
			)
		),
		modRuntime(GameModsRuntime::Build(rulesRuntime))
	{
	}

	virtual bool InitialiseLuaApi() = 0;
	virtual bool InitialiseLuaEvents() = 0;
	virtual void RegisterMods() = 0;

	virtual void Initialise()
	{
		EnhancementKeys::InitIfNeeded();

		rulesRuntime.EnsureRulesIniIsLoaded();

		rulesInitSuccessful = rulesRuntime.GetRules().IsValid();
		luaInitSuccessful = LoadLuaComponents();

		InitialiseMods();
	}

public:
	~NcoRuntime()
	{
		delete &rulesRuntime;
		delete &luaRuntime;
		delete &modRuntime;
	}

	bool RulesInitWasSuccessful()
	{
		return rulesInitSuccessful;
	}

	bool LuaInitWasSuccessful()
	{
		return luaInitSuccessful;
	}

	IRulesRuntime& GetRulesRuntime()
	{
		return rulesRuntime;
	}

	ILuaRuntime& GetLuaRuntime()
	{
		return luaRuntime;
	}

	GameModsRuntime& GetModRuntime()
	{
		return modRuntime;
	}

	void InitialiseModTypesIfRequired()
	{
		if (!modsInitialised)
		{
			return;
		}

		modsInitialised = true;


		modRuntime.InitaliseTypes();
	}
};
