#pragma once

#include <IRulesRuntime.h>

#include "TiberianDawnTypeMod.h"

template<class T, class U> class TechnoTypeMod : public TiberianDawnTypeMod<T, U>
{
protected:
	TechnoTypeMod(
		IRulesRuntime& runtime,
		const char* modTypeName,
		RuleName modTypesRuleName,
		RuleName modTypeCountRuleName,
		const RulesIniRuleKey& typeCountRuleKey,
		T totalTypeCount
	) : TiberianDawnTypeMod(
		runtime,
		modTypeName,
		modTypesRuleName,
		modTypeCountRuleName,
		typeCountRuleKey,
		totalTypeCount
	)
	{
	}

	U* CloneType(const char* baseTypeString, const char* typeString, T baseType, T type)
	{
		auto newType = TiberianDawnTypeMod::CloneType(baseTypeString, typeString, baseType, type);

		newType->HouseListCsv = strdup(newType->HouseListCsv);

		return newType;
	}

};
