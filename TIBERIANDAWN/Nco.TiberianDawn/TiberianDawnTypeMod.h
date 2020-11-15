#pragma once

#include <IRulesRuntime.h>
#include <NcoGameMod.h>

template<class T, class U> class TiberianDawnTypeMod : public NcoGameMod<T, U>
{
protected:
	TiberianDawnTypeMod(
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

	U* GetTypeInstance(T type)
	{
		return (U*)U::By_Type(type);
	}

	U* CloneType(const char* baseTypeString, const char* typeString, T baseType, T type)
	{
		auto newType = NcoGameMod::CloneType(baseTypeString, typeString, baseType, type);

		newType->IsModType = true;

		strcpy(newType->ModBaseIniName, baseTypeString);
		strcpy(newType->IniName, typeString);

		newType->Type = type;

		newType->FriendlyName = strdup(newType->FriendlyName);

		return newType;
	}

};
