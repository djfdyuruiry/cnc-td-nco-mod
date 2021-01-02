#pragma once

#include <functional>

#include <ParseCheckValidator.h>
#include <IRulesIniSection.h>

#include "../TYPE.H"

#include "rules_ini_unit.h"
#include "TechnoTypeApi.h"
#include "TiberianDawnNcoRuntime.h"

#define EXTRACTOR_UNT(f) EXTRACTOR(UnitTypeClass, f)
#define INJECTOR_UNT(t, f) INJECTOR(UnitTypeClass, t, f)
#define SIMPLE_EXTRACTOR_UNT(f) SIMPLE_EXTRACTOR(UnitTypeClass, f)
#define SIMPLE_INJECTOR_UNT(t, f) SIMPLE_INJECTOR(UnitTypeClass, t, f)

class UnitApi : public TechnoTypeApi<UnitTypeClass, UnitType>
{
private:
	UnitApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		TechnoTypeApi(strdup("Units"), rulesInfo, UNIT_FIRST, getCount)
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
			EXTRACTOR_UNT(NcoTypeConverter().ToStringOrDefault(i.Speed)),
			[](UnitTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Speed = NcoTypeConverter().ParseOrDefault(
					va.Read<const char*>(l, si),
					i.Speed
				);
			},
			ParseCheckValidator<SpeedType>::Build("Unit Speed", [](auto stringValue) {
				return &NcoTypeConverter().Parse<SpeedType>(stringValue);
			})
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo, std::function<int(void)> getCount)
	{
		return *(new UnitApi(rulesInfo, getCount));
	}

};
