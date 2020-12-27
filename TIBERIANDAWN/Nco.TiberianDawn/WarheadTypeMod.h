#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "TiberianDawnTypeMod.h"

class WarheadTypeMod : public TiberianDawnTypeMod<WarheadType, WarheadTypeClass>
{
private:
	WarheadTypeMod(IRulesRuntime& runtime)
		: TiberianDawnTypeMod(
			runtime,
			"Warhead",
			NEW_WARHEADS_RULE,
			NEW_WARHEAD_COUNT_RULE,
			*WARHEAD_COUNT_RULE_KEY,
			WARHEAD_COUNT,
			NcoTypeConverter().GetModTypeMap<WarheadType>()
		  )
	{
	}

protected:
	ResultWithValue<WarheadType>& ParseType(SectionName typeString)
	{
		return NcoTypeConverter().Parse<WarheadType>(typeString);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWarheadSection(typeString);
	}

	WarheadTypeClass* CloneType(const char* baseTypeString, const char* typeString, WarheadType baseType, WarheadType type)
	{
		auto newType = TiberianDawnTypeMod::CloneType(baseTypeString, typeString, baseType, type);
		auto newModifier = new unsigned[ARMOR_COUNT];

		for (auto i = 0u; i < ARMOR_COUNT; i++)
		{
			newModifier[i] = newType->Modifier[i];
		}

		newType->Modifier = newModifier;

		return newType;
	}

	void ReadRulesAndAddType(WarheadTypeClass* type)
	{
		WarheadTypeClass::ReadWarheadRules(type);

		WarheadTypeClass::Add_Warhead_Type(type);
	}

public:
	static WarheadTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new WarheadTypeMod(runtime));
	}

};
