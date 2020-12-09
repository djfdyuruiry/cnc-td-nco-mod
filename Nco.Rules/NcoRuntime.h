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
			auto scriptFilePath = BuildModDataFilePath(scriptFile);
			auto& executeResult = luaRuntime.ExecuteFile(scriptFilePath);

			if (executeResult.IsErrorResult())
			{
				LogError("Error loading rules lua script '%s': %s", scriptFilePath, executeResult.GetError());

				delete &executeResult;

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

		if (!InitialiseLuaCppApi())
		{
			LogError("Lua setup failed: c++ APIs could not be initialised");
			return false;
		}

		if (!InitialiseLuaNativeApi())
		{
			LogError("Lua setup failed: native APIs could not be initialised");
			return false;
		}

		if (!LoadLuaScripts())
		{
			LogError("Lua setup failed: script(s) from rules file could not be loaded");
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
					LuaStateFactory::Build(GetModDataPath())
				)
			)
		),
		modRuntime(GameModsRuntime::Build(rulesRuntime)),
		threads(*(new std::vector<Thread*>()))
	{
	}

	virtual bool InitialiseLuaCppApi() = 0;
	virtual bool InitialiseLuaNativeApi() = 0;
	virtual void RegisterMods() = 0;
	virtual void RegisterThreads() = 0;

	template<class T> NcoRuntime& RegisterThread(T& thread)
	{
		LogDebug("Registering thread: %s", thread.GetName());

		threads.push_back(&thread);

		return *this;
	}

	template<class T> NcoRuntime& RegisterThread()
	{
		auto& thread = T::Build();

		return RegisterThread<T>(thread);
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
		LogInfo("New Construction Options mod starting up");

		auto& runtime = T::GetInstance();

		if (!runtime.RulesInitWasSuccessful())
		{
			ShowError("NCO startup failed: rules INI failed validation.\n\nPlease check your rules are valid.");
			return false;
		}

		if (runtime.GetRulesRuntime().LuaIsEnabled() && !runtime.LuaInitWasSuccessful())
		{
			ShowError("NCO startup failed: errors initialising Lua");
			return false;
		}
		else
		{
			LogWarn("Lua is not enabled in rules file - scripts will be ignored and NOT run");
		}

		if (!runtime.GetModRuntime().InitaliseTypes())
		{
			ShowError("NCO startup failed: mods types setup failed validation.\n\nPlease check your mode type rules are valid.");
			return false;
		}

		if (!runtime.StartThreadsIfRequired())
		{
			ShowError("NCO startup failed: failed to start background threads");
			return false;
		}

		LogInfo("New Construction Options mod has started successfully");
		return true;
	}

	template<class T> static void Shutdown()
	{
		LogInfo("New Construction Options mod shutting down");

		T::Shutdown();

		// this must be the last call - otherwise the file might be reopened by a log call
		Logger::Shutdown();
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
