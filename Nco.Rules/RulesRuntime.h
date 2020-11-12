#pragma once

#include <vector>

#include <logger.h>

#include "lua.h"
#include "IRulesIni.h"
#include "rules_ini_nco.h"
#include "rules_ini_enhancements.h"
#include "rules_ini_mods.h"
#include "RulesIni.h"
#include "IRulesIniReader.h"
#include "IRulesRuntime.h"
#include "RulesIniReader.h"
#include "RulesIniRuleKey.h"
#include "RulesIniSectionBuilder.h"

template <class T> class RulesRuntime : public IRulesRuntime
{
private:
	static const auto ONE_SEC_IN_MILLIS = 1000;

	IRulesIni* rules;
	IRulesIniReader* rulesReader;

	bool luaIsEnabled;
	bool luaConsoleIsEnabled;
	unsigned int tickIntervalInMillis;
	LogLevel currentLogLevel;
	const char* logFilePath;
	std::vector<const char*>& luaScripts;

	RulesRuntime(unsigned int gameTicksPerSecond) :
		tickIntervalInMillis(ONE_SEC_IN_MILLIS / gameTicksPerSecond),
		rules(NULL),
		rulesReader(NULL),
		luaScripts(*(new std::vector<const char*>()))
	{
	}

	void ReadLuaSettings()
	{
		Log_Info("Reading Lua settings from rules ini");

		luaIsEnabled = rulesReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE);
		luaConsoleIsEnabled = luaIsEnabled
			&& rulesReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE);
		tickIntervalInMillis = rulesReader->ReadRuleValue<int>(NCO_RULES_SECTION_NAME, GAME_TICK_INTERVAL_IN_MS_RULE);

		auto onScenarioLoadCsv = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE);

		if (String_Is_Empty(onScenarioLoadCsv))
		{
			return;
		}

		auto scriptFileCount = 0u;
		auto scriptFiles = Parse_Csv_String(
			onScenarioLoadCsv,
			MAX_PATH,
			&scriptFileCount
		);

		for (auto i = 0u; i < scriptFileCount; i++)
		{
			luaScripts.push_back(scriptFiles[i]);
		}
	}

	void ReadLogSettings()
	{
		Log_Info("Parsing log level and file path from rules ini");

		auto logLevelBuffer = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogLevel");

		Convert_String_To_Upper_Case(logLevelBuffer);

		currentLogLevel = Parse_Log_Level(logLevelBuffer);
		logFilePath = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogFile");

		delete logLevelBuffer;
	}

	void DefineRulesSections() {
		auto& ncoSection = RulesIniSection::BuildSection(NCO_RULES_SECTION_NAME)
			.WithDefaultType(STRING_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << s.BuildRule("LogLevel")
					  .OnlyAccept(std::vector<const char*> {
						   Log_Level_To_String(OFF),
						   Log_Level_To_String(ERR),
						   Log_Level_To_String(WARN),
						   Log_Level_To_String(INFO),
						   Log_Level_To_String(DEBUG),
						   Log_Level_To_String(TRACE)
					   })
					   .WithDefault("OFF")

				  << "LogFile"  
				  << LUA_SCRIPTS_RULE
				  << ENABLE_LUA_SCRIPTS_RULE  << BOOL_RULE << true
				  << ENABLE_LUA_CONSOLE_RULE  << BOOL_RULE;
			});
			
		ncoSection << ncoSection.BuildRule(GAME_TICK_INTERVAL_IN_MS_RULE)
						.OfType(UNSIGNED_INT_RULE)
						.WithDefault(tickIntervalInMillis);

		*rules << ncoSection

			   << RulesIniSectionBuilder::BuildEnhancementRules()

			   << RulesIniSectionBuilder::BuildDifficultyRules(
					"Easy",
					1.2,
					1.2,
					1.2,
					0.3,
					0.8,
					0.8,
					0.001,
					0.001,
					0.6,
					false
			   )

			   << RulesIniSectionBuilder::BuildDifficultyRules(
					"Normal",
					1.0,
					1.0,
					1.0,
					1.0,
					1.0,
					1.0,
					0.02,
					0.03,
					1.0,
					true
			   )

			   << RulesIniSectionBuilder::BuildDifficultyRules(
					"Hard",
					0.9,
					0.9,
					0.9,
					1.05,
					1.05,
					1.0,
					1.0,
					0.05,
					0.1,
					true
			   )

			   << RulesIniSectionBuilder::BuildAiRules()
			   << RulesIniSectionBuilder::BuildIqRules();
	}

public:
	static RulesRuntime& Build(unsigned int gameTicksPerSecond)
	{
		return *(new RulesRuntime(gameTicksPerSecond));
	}

	~RulesRuntime()
	{
		if (rules != NULL)
		{
			delete rules;
		}

		if (rulesReader != NULL)
		{
			delete rulesReader;
		}

		delete &luaScripts;
	}

	void EnsureRulesIniIsLoaded()
	{
		if (rules != NULL)
		{
			return;
		}

		rules = &RulesIni::SourceRulesFrom("RULES.INI").AndThenFrom("RULES-DEFAULT.INI");

		DefineRulesSections();

		rulesReader = &(T::Build(*rules));

		ReadLogSettings();
		ReadLuaSettings();
	}

	IRulesIni& GetRules()
	{
		EnsureRulesIniIsLoaded();

		return *rules;
	}

	IRulesIniReader& GetBaseRulesReader()
	{
		return *rulesReader;
	}

	T& GetRulesReader()
	{
		EnsureRulesIniIsLoaded();

		return *((T*)rulesReader);
	}

	bool LuaIsEnabled()
	{
		return luaIsEnabled;
	}

	bool LuaConsoleIsEnabled()
	{
		return luaConsoleIsEnabled;
	}

	unsigned int GetGameTickIntervalInMs()
	{
		return tickIntervalInMillis;
	}

	const std::vector<const char*>& GetLuaScripts()
	{
		return luaScripts;
	}

	LogLevel GetCurrentLogLevel()
	{
		return currentLogLevel;
	}

	const char* GetLogFilePath()
	{
		return logFilePath;
	}
};
