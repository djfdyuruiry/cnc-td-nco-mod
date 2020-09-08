#pragma once

#include "function.h"

#include "NcoGameMod.h"

class BuildingTypeMod : public NcoGameMod<StructType, BuildingTypeClass>
{
private:
	BuildingTypeMod() 
		: NcoGameMod(
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
		GetRules() << RulesIniSectionBuilder::BuildBuildingSection(typeString);
	}

	void ReadRulesAndAddType(BuildingTypeClass* type)
	{
		BuildingTypeClass::Read_Building_Rules(type);

		BuildingTypeClass::Add_Building_Type(type);
	}

public:
	static BuildingTypeMod& Build()
	{
		return *(new BuildingTypeMod());
	}

};
