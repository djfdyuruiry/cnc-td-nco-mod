#pragma once

#include <IRulesRuntime.h>
#include <NcoGameMod.h>

template<class T, class U> class TechnoTypeMod : public NcoGameMod<T, U>
{
protected:
	TechnoTypeMod(
		IRulesRuntime& runtime,
		const char* modTypeName,
		RuleName modTypesRuleName,
		RuleName modTypeCountRuleName,
		const RulesIniRuleKey& typeCountRuleKey,
		T totalTypeCount
	) : NcoGameMod(
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
		auto newType = NcoGameMod::CloneType(baseTypeString, typeString, baseType, type);

		newType->HouseListCsv = strdup(newType->HouseListCsv);
		newType->FriendlyName = strdup(newType->FriendlyName);

		return newType;
	}

};