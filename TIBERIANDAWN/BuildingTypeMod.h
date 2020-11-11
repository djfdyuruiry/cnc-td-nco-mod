#pragma once

#include <NcoGameMod.h>

#include "function.h"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"

class BuildingTypeMod : public NcoGameMod<StructType, BuildingTypeClass>
{
private:
	BuildingTypeMod(IRulesRuntime& runtime)
		: NcoGameMod(
			runtime,
			"Building",
			NEW_BUILDINGS_RULE,
			NEW_BUILDING_COUNT_RULE,
			*BUILDING_COUNT_RULE_KEY,
			STRUCT_COUNT
		  )
	{
	}

protected:
	StructType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Structure_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildBuildingSection(typeString);
	}

	void ReadRulesAndAddType(BuildingTypeClass* type)
	{
		BuildingTypeClass::Read_Building_Rules(type);

		BuildingTypeClass::Add_Building_Type(type);
	}

public:
	static BuildingTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new BuildingTypeMod(runtime));
	}

};
