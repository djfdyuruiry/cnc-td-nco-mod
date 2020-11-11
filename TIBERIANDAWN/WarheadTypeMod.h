#pragma once

#include <NcoGameMod.h>

#include "function.h"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"

class WarheadTypeMod : public NcoGameMod<WarheadType, WarheadTypeClass>
{
private:
	WarheadTypeMod(IRulesRuntime& runtime)
		: NcoGameMod(
			runtime,
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
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWarheadSection(typeString);
	}

	void ReadRulesAndAddType(WarheadTypeClass* type)
	{
		WarheadTypeClass::Read_Warhead_Rules(type);

		WarheadTypeClass::Add_Warhead_Type(type);
	}

public:
	static WarheadTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new WarheadTypeMod(runtime));
	}

};
