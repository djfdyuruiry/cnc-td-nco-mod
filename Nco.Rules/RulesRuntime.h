#pragma once

#include <vector>

#include <Logger.h>

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

	std::function<void(IRulesIni&)> extensionSectionsSetup;
	std::function<IRulesIniReader*(IRulesIni*)> rulesReaderBuilder;

	bool luaIsEnabled;
	bool luaConsoleIsEnabled;
	unsigned int tickIntervalInMillis;
	LogLevel currentLogLevel;
	const char* logFilePath;
	std::vector<const char*>& luaScripts;

	RulesRuntime(
		unsigned int gameTicksPerSecond,
		std::function<void(IRulesIni&)> extensionSectionsSetup,
		std::function<IRulesIniReader*(IRulesIni*)> rulesReaderBuilder
	) :
		rules(NULL),
		rulesReader(NULL),
		extensionSectionsSetup(extensionSectionsSetup),
		rulesReaderBuilder(rulesReaderBuilder),
		tickIntervalInMillis(ONE_SEC_IN_MILLIS / gameTicksPerSecond),
		luaScripts(*(new std::vector<const char*>()))
	{
	}

	void ReadLuaSettings()
	{
		LogInfo("Reading Lua settings from rules ini");

		luaIsEnabled = rulesReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE);
		luaConsoleIsEnabled = luaIsEnabled && rulesReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE);
		tickIntervalInMillis = rulesReader->ReadRuleValue<int>(NCO_RULES_SECTION_NAME, GAME_TICK_INTERVAL_IN_MS_RULE);

		auto onScenarioLoadCsv = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE);

		if (StringIsEmpty(onScenarioLoadCsv))
		{
			return;
		}

		auto scriptFileCount = 0u;
		auto scriptFiles = ParseCsvString(
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
		LogInfo("Parsing log level and file path from rules ini");

		auto logLevelBuffer = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogLevel");

		ConvertStringToUpperCase(logLevelBuffer);

		currentLogLevel = ParseLogLevel(logLevelBuffer);
		logFilePath = rulesReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogFile");

		delete logLevelBuffer;
	}

	void DefineRulesSections() {
		auto& ncoSection = RulesIniSection::BuildSection(NCO_RULES_SECTION_NAME)
			.WithDefaultType(STRING_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << s.BuildRule("LogLevel")
					  .OnlyAccept(std::vector<const char*> {
						   LogLevelToString(OFF),
						   LogLevelToString(ERR),
						   LogLevelToString(WARN),
						   LogLevelToString(INFO),
						   LogLevelToString(DEBUG),
						   LogLevelToString(TRACE)
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

		IRulesIni& rulesRef = *rules;

		extensionSectionsSetup(rulesRef);
	}

public:
	static RulesRuntime& Build(
		unsigned int gameTicksPerSecond,
		std::function<void(IRulesIni&)> extensionSectionsSetup,
		std::function<IRulesIniReader*(IRulesIni*)> rulesReaderBuilder
	)
	{
		return *(new RulesRuntime(gameTicksPerSecond, extensionSectionsSetup, rulesReaderBuilder));
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

	bool LoadRulesIfRequired()
	{
		if (rules != NULL)
		{
			return rules->IsValid();
		}

		rules = &RulesIni::SourceRulesFrom("RULES.INI").AndThenFrom("RULES-DEFAULT.INI");

		DefineRulesSections();

		rulesReader = rulesReaderBuilder(rules);

		ReadLogSettings();
		ReadLuaSettings();

		return rules->IsValid();
	}

	IRulesIni& GetRules()
	{
		return *rules;
	}

	IRulesIniReader& GetBaseRulesReader()
	{
		return *rulesReader;
	}

	T& GetRulesReader()
	{
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
