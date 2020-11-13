#pragma once

#include <functional>

#include <ParseCheckValidator.h>
#include <IRulesIniSection.h>

#include "parse.h"
#include "rules_ini_unit.h"
#include "TechnoTypeApi.h"
#include "type.h"

#define EXTRACTOR_UNT(f) EXTRACTOR(UnitTypeClass, f)
#define INJECTOR_UNT(t, f) INJECTOR(UnitTypeClass, t, f)
#define SIMPLE_EXTRACTOR_UNT(f) SIMPLE_EXTRACTOR(UnitTypeClass, f)
#define SIMPLE_INJECTOR_UNT(t, f) SIMPLE_INJECTOR(UnitTypeClass, t, f)

class UnitApi : public TechnoTypeApi<UnitTypeClass, UnitType>
{
private:
	UnitApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		TechnoTypeApi(strdup("Unit"), rulesInfo, UNIT_FIRST, getCount, Parse_Unit_Type, Unit_Type_To_String)
	{
		technoTypeWrapper.WithFieldWrapper(
			CAN_BE_FOUND_IN_CRATE_RULE,
			EXTRACTOR_UNT((bool)i.IsCrateGoodie),
			SIMPLE_INJECTOR_UNT(bool, IsCrateGoodie),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			LIMITED_FACINGS_RULE,
			EXTRACTOR_UNT((bool)i.IsPieceOfEight),
			SIMPLE_INJECTOR_UNT(bool, IsPieceOfEight),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_CRUSH_INFANTRY_RULE,
			EXTRACTOR_UNT((bool)i.IsCrusher),
			SIMPLE_INJECTOR_UNT(bool, IsCrusher),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_HARVEST_RULE,
			EXTRACTOR_UNT((bool)i.IsToHarvest),
			SIMPLE_INJECTOR_UNT(bool, IsToHarvest),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			RADAR_EQUIPPED_RULE,
			EXTRACTOR_UNT((bool)i.IsRadarEquipped),
			SIMPLE_INJECTOR_UNT(bool, IsRadarEquipped),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_FIRE_ANIMATION_RULE,
			EXTRACTOR_UNT((bool)i.IsFireAnim),
			SIMPLE_INJECTOR_UNT(bool, IsFireAnim),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			LOCK_TURRET_RULE,
			EXTRACTOR_UNT((bool)i.IsLockTurret),
			SIMPLE_INJECTOR_UNT(bool, IsLockTurret),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_TRACKS_RULE,
			EXTRACTOR_UNT((bool)i.IsTracked),
			SIMPLE_INJECTOR_UNT(bool, IsTracked),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HUGE_RULE,
			EXTRACTOR_UNT((bool)i.IsGigundo),
			SIMPLE_INJECTOR_UNT(bool, IsGigundo),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CAN_CLOAK_RULE,
			EXTRACTOR_UNT((bool)i.IsCloakable),
			SIMPLE_INJECTOR_UNT(bool, IsCloakable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CONSTANT_ANIMATION_RULE,
			EXTRACTOR_UNT((bool)i.IsAnimating),
			SIMPLE_INJECTOR_UNT(bool, IsAnimating),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_TRACKS_RULE,
			EXTRACTOR_UNT((bool)i.IsTracked),
			SIMPLE_INJECTOR_UNT(bool, IsTracked),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			SELF_REPAIR_RULE,
			EXTRACTOR_UNT((bool)i.CanSelfRepair),
			SIMPLE_INJECTOR_UNT(bool, CanSelfRepair),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			UNIT_SPEED_RULE,
			EXTRACTOR_UNT(Unit_Speed_Type_To_String(i.Speed)),
			[](UnitTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

				i.Speed = Parse_Unit_Speed_Type(valueUpper, NULL);

				delete valueUpper;
			},
			ParseCheckValidator<SpeedType>::Build("Unit Speed", Parse_Unit_Speed_Type)
		).WithFieldWrapper(
			RATE_OF_TURN_RULE,
			SIMPLE_EXTRACTOR_UNT(ROT),
			SIMPLE_INJECTOR_UNT(unsigned char, ROT),
			NumbericRangeValidator<>::Build(0, UCHAR_MAX)
		).WithFieldWrapper(
			TRANSPORT_CAPACITY_RULE,
			SIMPLE_EXTRACTOR_UNT(TransportCapacity),
			SIMPLE_INJECTOR_UNT(unsigned int, TransportCapacity),
			NumbericRangeValidator<>::Build(0, UINT_MAX)
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo, std::function<int(void)> getCount)
	{
		return *(new UnitApi(rulesInfo, getCount));
	}

};
