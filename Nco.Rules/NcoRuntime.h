#pragma once

#include <stdlib.h>
#include <vector>

#include <ILuaRuntime.h>
#include <LuaRuntime.h>
#include <LuaStateFactory.h>
#include <LuaStateWrapper.h>
#include <Thread.h>
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

		modRuntime.ReadTypesIfRequired();
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
			Log_Error("Lua setup failed: API could not be initialised");
			return false;
		}

		if (!InitialiseLuaEvents())
		{
			Log_Error("Lua setup failed: events could not be initialised");
			return false;
		}

		if (!LoadLuaScripts())
		{
			Log_Error("Lua setup failed: script(s) from rules file could not be loaded");
			return false;
		}

		return true;
	}

protected:
	bool rulesInitSuccessful;
	bool luaInitSuccessful;
	bool modTypesInitialised;
	bool threadsStarted;
	IRulesRuntime& rulesRuntime;
	ILuaRuntime& luaRuntime;
	GameModsRuntime& modRuntime;
	std::vector<Thread*>& threads;

	NcoRuntime(IRulesRuntime& rulesRuntime) :
		rulesRuntime(rulesRuntime),
		luaRuntime(
			LuaRuntime::Build(
				LuaStateWrapper::Build(
					LuaStateFactory::Build(Get_Mod_Data_Path())
				)
			)
		),
		modRuntime(GameModsRuntime::Build(rulesRuntime)),
		threads(*(new std::vector<Thread*>()))
	{
	}

	virtual bool InitialiseLuaApi() = 0;
	virtual bool InitialiseLuaEvents() = 0;
	virtual void RegisterMods() = 0;
	virtual void RegisterThreads() = 0;

	template<class T> NcoRuntime& RegisterThread()
	{
		auto& thread = T::Build();

		Log_Debug("Registering thread: %s", thread.GetName());

		threads.push_back(&thread);

		return *this;
	}

	virtual void Initialise()
	{
		EnhancementKeys::InitIfNeeded();

		rulesInitSuccessful = rulesRuntime.LoadRulesIfRequired();
		luaInitSuccessful = LoadLuaComponents();

		InitialiseMods();
		RegisterThreads();
	}

	bool InitialiseModTypesIfRequired()
	{
		if (modTypesInitialised)
		{
			return true;
		}

		modTypesInitialised = modRuntime.InitaliseTypes();

		return modTypesInitialised;
	}

	bool StartThreadsIfRequired()
	{
		if (threadsStarted)
		{
			return true;
		}

		threadsStarted = true;

		for (auto thread : threads)
		{
			if (!thread->Start())
			{
				threadsStarted = false;
			}
		}

		return threadsStarted;
	}

	virtual bool InternalShutdown()
	{
		auto shutdownSuccess = true;

		for (auto thread : threads)
		{
			if (!thread->Stop())
			{
				shutdownSuccess = false;
			}
		}

		return shutdownSuccess;
	}

public:
	template<class T> static bool Startup()
	{
		Log_Info("New Construction Options mod starting up");

		auto& runtime = T::GetInstance();

		if (!runtime.RulesInitWasSuccessful())
		{
			Show_Error("NCO startup failed: rules INI failed validation.\n\nPlease check your rules are valid.");
			return false;
		}

		if (runtime.GetRulesRuntime().LuaIsEnabled() && !runtime.LuaInitWasSuccessful())
		{
			Show_Error("NCO startup failed: errors initialising Lua");
			return false;
		}
		else
		{
			Log_Warn("Lua is not enabled in rules file - scripts will be ignored and NOT run");
		}

		if (!runtime.GetModRuntime().InitaliseTypes())
		{
			Show_Error("NCO startup failed: mods types setup failed validation.\n\nPlease check your mode type rules are valid.");
			return false;
		}

		if (!runtime.StartThreadsIfRequired())
		{
			Show_Error("NCO startup failed: failed to start background threads");
			return false;
		}

		Log_Info("New Construction Options mod has started successfully");
		return true;
	}

	template<class T> static void Shutdown()
	{
		Log_Info("New Construction Options mod shutting down");

		T::Shutdown();

		// this must be the last call - otherwise the file might be reopened by a log call
		Close_Log_File_If_Open();
	}

	~NcoRuntime()
	{
		delete &rulesRuntime;
		delete &luaRuntime;
		delete &modRuntime;

		for (auto thread : threads)
		{
			delete thread;
		}

		delete &threads;
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

};
