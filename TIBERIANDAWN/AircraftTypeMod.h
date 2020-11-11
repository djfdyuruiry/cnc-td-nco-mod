#pragma once

#include <NcoGameMod.h>
#include <IRulesRuntime.h>

#include "tiberian_dawn_mods.h"
#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRulesReader.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "type.h"

class AircraftTypeMod : public NcoGameMod<AircraftType, AircraftTypeClass>
{
private:
	AircraftTypeMod(IRulesRuntime& runtime)
		: NcoGameMod(
			runtime,
			"Aircraft",
			NEW_AIRCRAFT_RULE,
			NEW_AIRCRAFT_COUNT_RULE,
			*AIRCRAFT_COUNT_RULE_KEY,
			AIRCRAFT_COUNT
		  )
	{
	}

protected:
	AircraftType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Aircraft_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildAircraftSection(typeString);
	}

	void ReadRulesAndAddType(AircraftTypeClass* type)
	{
		AircraftTypeClass::Read_Aircraft_Rules(type);

		AircraftTypeClass::Add_Aircraft_Type(type);
	}

public:
	static AircraftTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new AircraftTypeMod(runtime));
	}

};
