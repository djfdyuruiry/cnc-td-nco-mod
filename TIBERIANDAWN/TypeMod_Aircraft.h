#pragma once

#include "function.h"

#include "NcoGameMod.h"

class AircraftTypeMod : public NcoGameMod<AircraftType, AircraftTypeClass>
{
private:
	AircraftTypeMod() 
		: NcoGameMod(
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
		return Parse_Aircraft_Type(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		GetRules() << RulesIniSectionBuilder::BuildAircraftSection(typeString);
	}

	void ReadRulesAndAddType(AircraftTypeClass* type)
	{
		AircraftTypeClass::Read_Aircraft_Rules(type);

		AircraftTypeClass::Add_Aircraft_Type(type);
	}

public:
	static AircraftTypeMod& Build()
	{
		return *(new AircraftTypeMod());
	}

};
