#pragma once

#include "function.h"

#include "NcoGameMod.h"

class UnitTypeMod : public NcoGameMod<UnitType, UnitTypeClass>
{
private:
	UnitTypeMod()
		: NcoGameMod(
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
		return Parse_Unit_Type(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		GetRules() << RulesIniSectionBuilder::BuildUnitSection(typeString);
	}

	void ReadRulesAndAddType(UnitTypeClass* type)
	{
		UnitTypeClass::Read_Unit_Rules(type);

		UnitTypeClass::Add_Unit_Type(type);
	}

public:
	static UnitTypeMod& Build()
	{
		return *(new UnitTypeMod());
	}

};
