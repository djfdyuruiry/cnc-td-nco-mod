#pragma once

#include <LuaObjectUtils.h>
#include <TypeWrapperApi.h>
#include <TypePatterns.h>

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
		char* typeName,
		IRulesIniSection& rulesInfo,
		U first,
		std::function<int(void)> getCount,
		std::function<ResultWithValue<U>*(const char*)> typeParser,
		std::function<ResultWithValue<const char*>*(U)> typeToString
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
