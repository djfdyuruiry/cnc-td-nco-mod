#pragma once

#include <TypeWrapperApi.h>

#include "IRulesIniSection.h"

template<class T, class U> class RulesSectionTypeWrapperApi : public TypeWrapperApi<T, U>
{
protected:
	IRulesIniSection& rulesInfo;

	RulesSectionTypeWrapperApi(const char* typeName, IRulesIniSection& rulesInfo, U(*typeParser)(const char*, bool*, bool)) :
		TypeWrapperApi(typeName, typeParser),
		rulesInfo(rulesInfo)
	{
	}

	bool ValidateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

};
