#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TechnoTypeMod.h"
#include "TiberianDawnRuleSectionBuilder.h"

class UnitTypeMod : public TechnoTypeMod<UnitType, UnitTypeClass>
{
private:
	UnitTypeMod(IRulesRuntime& runtime)
		: TechnoTypeMod(
			runtime,
			"Unit",
			NEW_UNITS_RULE,
			NEW_UNIT_COUNT_RULE,
			*UNIT_COUNT_RULE_KEY,
			UNIT_COUNT,
			NcoTypeConverter().GetModTypeMap<UnitType>()
		)
	{
	}

protected:
	ResultWithValue<UnitType>& ParseType(SectionName typeString)
	{
		return NcoTypeConverter().Parse<UnitType>(typeString);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildUnitSection(typeString);
	}

	void ReadRulesAndAddType(UnitTypeClass* type)
	{
		UnitTypeClass::ReadUnitRules(type);

		UnitTypeClass::Add_Unit_Type(type);
	}

public:
	static UnitTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new UnitTypeMod(runtime));
	}

};
