#pragma once

#include <functional>

#include <lua.hpp>

#include "IRulesIniSection.h"
#include "parse.h"
#include "ParseCheckValidator.h"
#include "rules_ini_aircraft.h"
#include "TechnoTypeApi.h"
#include "type.h"

#define EXTRACTOR_AIR(f) EXTRACTOR(AircraftTypeClass, f)
#define INJECTOR_AIR(t, f) INJECTOR(AircraftTypeClass, t, f)
#define SIMPLE_EXTRACTOR_AIR(f) SIMPLE_EXTRACTOR(AircraftTypeClass, f)
#define SIMPLE_INJECTOR_AIR(t, f) SIMPLE_INJECTOR(AircraftTypeClass, t, f)

class AircraftApi : public TechnoTypeApi<AircraftTypeClass, AircraftType>
{
private:
	AircraftApi(IRulesIniSection& rulesInfo) : TechnoTypeApi("Aircraft", rulesInfo, Parse_Aircraft_Type)
	{
		technoTypeWrapper.WithFieldWrapper(
			CANT_HOVER_RULE,
			EXTRACTOR_AIR((bool)i.IsFixedWing),
			SIMPLE_INJECTOR_AIR(bool, IsFixedWing),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_LAND_RULE,
			EXTRACTOR_AIR((bool)i.IsLandable),
			SIMPLE_INJECTOR_AIR(bool, IsLandable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_ROTOR_RULE,
			EXTRACTOR_AIR((bool)i.IsRotorEquipped),
			SIMPLE_INJECTOR_AIR(bool, IsRotorEquipped),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			RATE_OF_TURN_RULE,
			SIMPLE_EXTRACTOR_AIR(ROT),
			SIMPLE_INJECTOR_AIR(unsigned char, ROT),
			NumbericRangeValidator<>::Build(0, UCHAR_MAX)
		).WithFieldWrapper(
			TRANSPORT_CAPACITY_RULE,
			SIMPLE_EXTRACTOR_AIR(TransportCapacity),
			SIMPLE_INJECTOR_AIR(unsigned int, TransportCapacity),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		).WithFieldWrapper(
			TRANSPORT_CAPACITY_RULE,
			SIMPLE_EXTRACTOR_AIR(TransportCapacity),
			SIMPLE_INJECTOR_AIR(unsigned int, TransportCapacity),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		);
	}

protected:

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo)
	{
		return *(new AircraftApi(rulesInfo));
	}

};
