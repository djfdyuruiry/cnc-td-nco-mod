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
			INFANTRY_COUNT,
			NcoTypeConverter().GetModTypeMap<InfantryType>()
		  )
	{
	}

protected:
	void AddType(InfantryTypeClass* instance)
	{
		InfantryTypeClass::Add_Infantry_Type(instance);
	}

public:
	static InfantryTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new InfantryTypeMod(runtime));
	}

};
