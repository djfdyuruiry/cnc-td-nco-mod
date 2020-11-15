#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TechnoTypeMod.h"
#include "TiberianDawnRuleSectionBuilder.h"

class InfantryTypeMod : public TechnoTypeMod<InfantryType, InfantryTypeClass>
{
private:
	InfantryTypeMod(IRulesRuntime& runtime) 
		: TechnoTypeMod(
			runtime,
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
		return Parse_Infantry_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildInfantrySection(typeString);
	}

	void ReadRulesAndAddType(InfantryTypeClass* type)
	{
		InfantryTypeClass::Read_Infantry_Rules(type);

		InfantryTypeClass::Add_Infantry_Type(type);
	}

public:
	static InfantryTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new InfantryTypeMod(runtime));
	}

};
