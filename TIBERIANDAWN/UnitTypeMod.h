#pragma once

#include <NcoGameMod.h>

#include "function.h"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"

class UnitTypeMod : public NcoGameMod<UnitType, UnitTypeClass>
{
private:
	UnitTypeMod(IRulesRuntime& runtime)
		: NcoGameMod(
			runtime,
			"Unit",
			NEW_UNITS_RULE,
			NEW_UNIT_COUNT_RULE,
			*UNIT_COUNT_RULE_KEY,
			UNIT_COUNT
		)
	{
	}

protected:
	UnitType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Unit_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildUnitSection(typeString);
	}

	void ReadRulesAndAddType(UnitTypeClass* type)
	{
		UnitTypeClass::Read_Unit_Rules(type);

		UnitTypeClass::Add_Unit_Type(type);
	}

public:
	static UnitTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new UnitTypeMod(runtime));
	}

};
