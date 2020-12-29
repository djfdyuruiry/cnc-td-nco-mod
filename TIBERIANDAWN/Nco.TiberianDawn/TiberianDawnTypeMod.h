#pragma once

#include <IRulesRuntime.h>
#include <NcoGameMod.h>
#include <TiberianDawnTypeConverter.h>
#include <TwoWayStringMap.h>

template<class T, class U> class TiberianDawnTypeMod : public NcoGameMod<T, U>
{
protected:
	TiberianDawnTypeMod(
		IRulesRuntime& runtime,
		const char* modTypeName,
		RuleName modTypesRuleName,
		RuleName modTypeCountRuleName,
		const RulesIniRuleKey& typeCountRuleKey,
		T totalTypeCount,
		TwoWayStringMap<T>& typeModMap
	) : NcoGameMod(
			runtime,
			modTypeName,
			modTypesRuleName,
			modTypeCountRuleName,
			typeCountRuleKey,
			totalTypeCount,
			typeModMap
		)
	{
	}

	ResultWithValue<T>& ParseType(SectionName typeString)
	{
		return NcoTypeConverter().Parse<T>(typeString);
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

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildSectionForType<U>(typeString);
	}

	virtual void AddType(U* instance) = 0;

	void ReadRulesAndAddType(U* newType)
	{
		auto& reader = TiberianDawnRulesReader::Build(runtime.GetRules(), NcoTypeConverter());

		newType->Read_Rules(reader);

		delete &reader;

		AddType(newType);
	}

};
