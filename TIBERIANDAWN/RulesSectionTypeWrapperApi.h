#pragma once

#include <LuaObjectUtils.h>
#include <TypeWrapperApi.h>

#include "IRulesIniSection.h"

template<class T, class U> class RulesSectionTypeWrapperApi : public TypeWrapperApi<T, U>
{
	static int GetSectionRulesProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(RulesSectionTypeWrapperApi, GetSectionRules);
	}

	int GetSectionRules(ILuaStateWrapper& lua)
	{
		lua.WriteArray(rulesInfo.GetRuleNames());

		return 1;
	}

protected:
	IRulesIniSection& rulesInfo;

	RulesSectionTypeWrapperApi(
		const char* typeName,
		IRulesIniSection& rulesInfo,
		U first,
		std::function<int(void)> getCount,
		U(*typeParser)(const char*, bool*, bool),
		const char* (*typeToString)(U, bool)
	) :
		TypeWrapperApi(typeName, first, getCount, typeParser, typeToString),
		rulesInfo(rulesInfo)
	{
		WithMethod(FormatString("get%sRuleNames", titleCaseTypeName), this, GetSectionRulesProxy)
			.WithDescription(FormatString("Get a list of %s rules", titleCaseTypeName));
	}

	bool ValidateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

};
