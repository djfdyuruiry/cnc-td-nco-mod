#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TechnoTypeMod.h"
#include "TiberianDawnRuleSectionBuilder.h"

class BuildingTypeMod : public TechnoTypeMod<StructType, BuildingTypeClass>
{
private:
	BuildingTypeMod(IRulesRuntime& runtime)
		: TechnoTypeMod(
			runtime,
			"Building",
			NEW_BUILDINGS_RULE,
			NEW_BUILDING_COUNT_RULE,
			*BUILDING_COUNT_RULE_KEY,
			STRUCT_COUNT,
			NcoTypeConverter().GetModTypeMap<StructType>()
		  )
	{
	}

protected:
	void AddType(BuildingTypeClass* instance)
	{
		BuildingTypeClass::Add_Building_Type(instance);
	}

public:
	static BuildingTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new BuildingTypeMod(runtime));
	}

};
