#pragma once

#include "function.h"

#include "NcoGameMod.h"

class WarheadTypeMod : public NcoGameMod<WarheadType, WarheadTypeClass>
{
private:
	WarheadTypeMod() 
		: NcoGameMod(
			"Warhead",
			NEW_WARHEADS_RULE,
			NEW_WARHEAD_COUNT_RULE,
			*WARHEAD_COUNT_RULE_KEY,
			WARHEAD_COUNT
		  )
	{
	}

protected:
	WarheadType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Warhead_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		GetRules() << RulesIniSectionBuilder::BuildWarheadSection(typeString);
	}

	void ReadRulesAndAddType(WarheadTypeClass* type)
	{
		WarheadTypeClass::Read_Warhead_Rules(type);

		WarheadTypeClass::Add_Warhead_Type(type);
	}

public:
	static WarheadTypeMod& Build()
	{
		return *(new WarheadTypeMod());
	}

};
