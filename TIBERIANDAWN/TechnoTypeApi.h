#pragma once

#include <map>

#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <TypeApi.h>

#include "IRulesIniSection.h"
#include "LuaTypeWrapper.h"
#include "RULES_CACHE_KEY.H"
#include "Type.h"

template<class T> class TechnoTypeApi : public TypeApi<T>
{
protected:
	IRulesIniSection& rulesInfo;
	std::map<CacheKey, CacheKey>& ruleToFieldMap;
	LuaTypeWrapper<T>& technoTypeWrapper;

	TechnoTypeApi(const char* typeName, IRulesIniSection& rulesInfo) :
		TypeApi(typeName),
		rulesInfo(rulesInfo),
		ruleToFieldMap(*(new std::map<CacheKey, CacheKey>())),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
		technoTypeWrapper.WithFieldWrapper(
			"Level",
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va) {
				va.Write(l, i.Level);
			},
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Level = va.Read<unsigned char>(l, si);
			},
			NumbericRangeValidator<unsigned char>::Build(0, 99)
		);

		ruleToFieldMap[Build_Rule_Key("BuildLevel")] = Build_Rule_Key("Level");
	}

	bool validateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

	CacheKey ResolveFieldKey(const char* ruleName)
	{
		auto ruleKey = Build_Rule_Key(ruleName);
		auto fieldKey = ruleKey;

		if (ruleToFieldMap.find(ruleKey) != ruleToFieldMap.end())
		{
			fieldKey = ruleToFieldMap[ruleKey];
		}

		return fieldKey;
	}

	bool ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName)
	{
		if (!validateRule(ruleName))
		{
			return false;
		}

		technoTypeWrapper.ReadFieldValue(ResolveFieldKey(ruleName), lua);

		return true;
	}

	bool WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex)
	{
		if (!validateRule(ruleName))
		{
			return false;
		}

		return technoTypeWrapper.WriteFieldValue(typeInstance, ResolveFieldKey(ruleName), lua, ruleValueStackIndex);
	}
};
