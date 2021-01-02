#pragma once

#include <functional>

#include <ParseCheckValidator.h>

#include "../DEFINES.H"
#include "../TYPE.H"

#include "IRulesIniSection.h"
#include "rules_ini_aircraft.h"
#include "TechnoTypeApi.h"
#include "TiberianDawnNcoRuntime.h"

#define EXTRACTOR_AIR(f) EXTRACTOR(AircraftTypeClass, f)
#define BOOL_EXTRACTOR_AIR(f) BOOL_EXTRACTOR(AircraftTypeClass, f)
#define INJECTOR_AIR(t, f) INJECTOR(AircraftTypeClass, t, f)
#define SIMPLE_EXTRACTOR_AIR(f) SIMPLE_EXTRACTOR(AircraftTypeClass, f)
#define SIMPLE_INJECTOR_AIR(t, f) SIMPLE_INJECTOR(AircraftTypeClass, t, f)

class AircraftApi : public TechnoTypeApi<AircraftTypeClass, AircraftType>
{
private:
	AircraftApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		TechnoTypeApi(strdup("Aircraft"), rulesInfo, AIRCRAFT_FIRST, getCount)
	{
		technoTypeWrapper.WithFieldWrapper(
			CANT_HOVER_RULE,
			BOOL_EXTRACTOR_AIR(IsFixedWing),
			SIMPLE_INJECTOR_AIR(bool, IsFixedWing),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_LAND_RULE,
			BOOL_EXTRACTOR_AIR(IsLandable),
			SIMPLE_INJECTOR_AIR(bool, IsLandable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_ROTOR_RULE,
			BOOL_EXTRACTOR_AIR(IsRotorEquipped),
			SIMPLE_INJECTOR_AIR(bool, IsRotorEquipped),
			PrimitiveTypeValidator<bool>::Build()
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo, std::function<int(void)> getCount)
	{
		return *(new AircraftApi(rulesInfo, getCount));
	}

};
