#pragma once

#include <stdlib.h>

#include <ILuaRuntime.h>
#include <LuaRuntime.h>
#include <LuaStateFactory.h>
#include <LuaStateWrapper.h>
#include <Utils.h>

#include "IRulesRuntime.h"
#include "RulesRuntime.h"
#include "RulesIniRuleKey.h"

class NcoRuntime
{
protected:
	bool rulesInitSuccessful;
	bool luaInitSuccessful;
	IRulesRuntime& rulesRuntime;
	ILuaRuntime& luaRuntime;

	NcoRuntime(IRulesRuntime& rulesRuntime) :
		rulesRuntime(rulesRuntime),
		luaRuntime(
			LuaRuntime::Build(
				LuaStateWrapper::Build(
					LuaStateFactory::Build(Get_Mod_Data_Path())
				)
			)
		)
	{
		InitaliseRuleKeys();
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

				delete &executeResult;

				executionOk = false;
			}
		}

		return executionOk;
	}

	virtual bool InitialiseLuaApi() = 0;
	virtual bool InitialiseLuaEvents() = 0;

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

	void Initialise()
	{
		rulesRuntime.EnsureRulesIniIsLoaded();

		rulesInitSuccessful = rulesRuntime.GetRules().IsValid();
		luaInitSuccessful = LoadLuaComponents();
	}

public:
	~NcoRuntime()
	{
		delete &rulesRuntime;
		delete &luaRuntime;
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

};
