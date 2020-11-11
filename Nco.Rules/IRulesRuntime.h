#pragma once

#include <logger.h>

#include "IRulesIni.h"
#include "RulesIniReader.h"
#include "RulesIniTypes.h"
#include "RulesIniRuleKey.h"

class IRulesRuntime
{
protected:
	IRulesRuntime()
	{
	}

public:
	virtual void EnsureRulesIniIsLoaded() = 0;

	virtual IRulesIni& GetRules() = 0;

	virtual RulesIniReader& GetBaseRulesReader() = 0;

	template<class T> T ReadRuleValue(SectionName section, RuleName rule, T defaultValue)
	{
		EnsureRulesIniIsLoaded();

		return GetBaseRulesReader().ReadRuleValue<T>(section, rule, defaultValue);
	}

	template<class T> T ReadRuleValue(SectionName section, RuleName rule)
	{
		EnsureRulesIniIsLoaded();

		return GetBaseRulesReader().ReadRuleValue<T>(section, rule);
	}

	template<class T> T ReadRuleValue(const RulesIniRuleKey& key)
	{
		EnsureRulesIniIsLoaded();

		return GetBaseRulesReader().ReadRuleValue<T>(key);
	}

	template<class T> T ReadRuleValue(const RulesIniRuleKey* key)
	{
		return ReadRuleValue<T>(*key);
	}

	virtual bool LuaIsEnabled() = 0;

	virtual bool LuaConsoleIsEnabled() = 0;

	virtual unsigned int GetGameTickIntervalInMs() = 0;

	virtual const std::vector<const char*>& GetLuaScripts() = 0;

	virtual LogLevel GetCurrentLogLevel() = 0;

	virtual const char* GetLogFilePath() = 0;
};
