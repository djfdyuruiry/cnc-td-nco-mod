#pragma once

#include <IRulesRuntime.h>

#include "../DEFINES.H"
#include "../TYPE.H"

#include "tiberian_dawn_mods.h"
#include "tiberian_dawn_rule_keys.h"
#include "TechnoTypeMod.h"
#include "TiberianDawnRulesReader.h"
#include "TiberianDawnRuleSectionBuilder.h"

class AircraftTypeMod : public TechnoTypeMod<AircraftType, AircraftTypeClass>
{
private:
	AircraftTypeMod(IRulesRuntime& runtime)
		: TechnoTypeMod(
			runtime,
			"Aircraft",
			NEW_AIRCRAFT_RULE,
			NEW_AIRCRAFT_COUNT_RULE,
			*AIRCRAFT_COUNT_RULE_KEY,
			AIRCRAFT_COUNT,
			NcoTypeConverter().GetModTypeMap<AircraftType>()
		  )
	{
	}

protected:
	ResultWithValue<AircraftType>& ParseType(SectionName typeString)
	{
		return NcoTypeConverter().Parse<AircraftType>(typeString);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildAircraftSection(typeString);
	}

	void ReadRulesAndAddType(AircraftTypeClass* type)
	{
		AircraftTypeClass::ReadAircraftRules(type);

		AircraftTypeClass::Add_Aircraft_Type(type);
	}

public:
	static AircraftTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new AircraftTypeMod(runtime));
	}

};
