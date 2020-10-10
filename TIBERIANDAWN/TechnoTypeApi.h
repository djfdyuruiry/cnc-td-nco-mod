#pragma once

#include <TypeApi.h>

#include "IRulesIniSection.h"
#include "Type.h"

class TechnoTypeApi : public TypeApi<TechnoTypeClass>
{
private:


	TechnoTypeApi(const char* typeName, IRulesIniSection& rulesInfo) : TypeApi(typeName), rulesInfo(rulesInfo)
	{
	}

protected:
	IRulesIniSection& rulesInfo;

	bool validateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

	bool ReadRule(TechnoTypeClass& typeInstance, const char* ruleName)
	{
	}

	bool WriteRule(TechnoTypeClass& typeInstance, const char* ruleName, const char* ruleValue)
	{
	}
};
