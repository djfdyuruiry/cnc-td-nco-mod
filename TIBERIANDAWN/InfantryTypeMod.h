#pragma once

#include "function.h"

#include "NcoGameMod.h"

class InfantryTypeMod : public NcoGameMod<InfantryType, InfantryTypeClass>
{
private:
	InfantryTypeMod() 
		: NcoGameMod(
			"Infantry",
			NEW_INFANTRY_RULE,
			NEW_INFANTRY_COUNT_RULE,
			*INFANTRY_COUNT_RULE_KEY,
			INFANTRY_COUNT
		  )
	{
	}

protected:
	InfantryType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Infantry_Type(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		GetRules() << RulesIniSectionBuilder::BuildInfantrySection(typeString);
	}

	void ReadRulesAndAddType(InfantryTypeClass* type)
	{
		InfantryTypeClass::Read_Infantry_Rules(type);

		InfantryTypeClass::Add_Infantry_Type(type);
	}

public:
	static InfantryTypeMod& Build()
	{
		return *(new InfantryTypeMod());
	}

};
