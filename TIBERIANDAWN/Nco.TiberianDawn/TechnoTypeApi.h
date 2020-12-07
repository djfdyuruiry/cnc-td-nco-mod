#pragma once

#include <IRulesIniSection.h>
#include <LambdaValidator.h>
#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <ParseCheckValidator.h>
#include <RulesSectionTypeWrapperApi.h>
#include <strings.h>

#include "../DEFINES.H"
#include "../TYPE.H"

#include "parse.h"
#include "rules_ini_generic.h"
#include "TiberianDawnNcoRuntime.h"

#define EXTRACTOR_T(f) EXTRACTOR(T, f)
#define INJECTOR_T(t, f) INJECTOR(T, t, f)
#define SIMPLE_EXTRACTOR_T(f) SIMPLE_EXTRACTOR(T, f)
#define SIMPLE_INJECTOR_T(t, f) SIMPLE_INJECTOR(T, t, f)

template<class T, class U> class TechnoTypeApi : public RulesSectionTypeWrapperApi<T, U>
{
protected:
	TechnoTypeApi(
		char* typeName,
		IRulesIniSection& rulesInfo,
		U first,
		std::function<int(void)> getCount
	) : RulesSectionTypeWrapperApi(
		typeName,
		rulesInfo,
		first,
		getCount,
		[](auto stringValue)
		{
			return &NcoTypeConverter().Parse<U>(stringValue);
		},
		[](auto value)
		{
			return &NcoTypeConverter().ToString(value);
		}
	)
	{
		technoTypeWrapper.WithFieldWrapper(
			BUILD_LEVEL_RULE,
			SIMPLE_EXTRACTOR_T(Level),
			SIMPLE_INJECTOR_T(unsigned char, Level),
			NumbericRangeValidator<unsigned char>::Build(0, 99)
		).WithFieldWrapper(
			SCENARIO_LEVEL_RULE,
			SIMPLE_EXTRACTOR_T(Scenario),
			SIMPLE_INJECTOR_T(unsigned char, Scenario),
			NumbericRangeValidator<unsigned char>::Build(0, 99)
		).WithFieldWrapper(
			PREREQUISITE_RULE,
			EXTRACTOR_T(PrerequisiteToString(i.Pre)),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				auto structType = NcoTypeConverter().ParseOrDefault(
					va.Read<const char*>(l, si),
					PrerequisiteToStructureType(i.Pre)
				);

				auto& preReqResult = StructureTypeToPrerequisite(structType);

				if (!preReqResult.IsErrorResult())
				{
					i.Pre = preReqResult.GetValue();
				}

				delete &preReqResult;
			},
			ParseCheckValidator<StructType>::Build("Building", [](auto stringValue) {
				return &NcoTypeConverter().Parse<StructType>(stringValue);
			})
		).WithFieldWrapper(
			COST_RULE,
			SIMPLE_EXTRACTOR_T(Cost),
			SIMPLE_INJECTOR_T(int, Cost),
			PrimitiveTypeValidator<int>::Build()
		).WithFieldWrapper(
			BUILDABLE_RULE,
			EXTRACTOR_T((bool)i.IsBuildable),
			SIMPLE_INJECTOR_T(bool, IsBuildable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			FLAMMABLE_RULE,
			EXTRACTOR_T((bool)i.IsFlammable),
			SIMPLE_INJECTOR_T(bool, IsFlammable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			SPEED_RULE,
			EXTRACTOR_T((unsigned char)i.MaxSpeed),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.MaxSpeed = (MPHType) va.Read<unsigned char>(l, si);
			},
			NumbericRangeValidator<>::Build(0, UCHAR_MAX)
		).WithFieldWrapper(
			STRENGTH_RULE,
			SIMPLE_EXTRACTOR_T(MaxStrength),
			SIMPLE_INJECTOR_T(unsigned short, MaxStrength),
			NumbericRangeValidator<>::Build(0, USHRT_MAX)
		).WithFieldsWrapper(
			std::vector<const char*> { OWNER_RULE, HOUSES_RULE },
			SIMPLE_EXTRACTOR_T(HouseListCsv),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				auto houseListCsv = va.Read<const char*>(l, si);
				auto& houseListResult = ParseHouseNameListCsv(houseListCsv);

				if (!houseListResult.IsErrorResult())
				{
					i.Ownable = houseListResult.GetValue();
					i.HouseListCsv = strdup(houseListCsv);
				}

				delete &houseListResult;
			},
			ParseCheckValidator<int>::Build("House List", [](auto stringValue) {
				return &ParseHouseNameListCsv(stringValue);
			})
		).WithFieldWrapper(
			PRIMARY_WEAPON_RULE,
			EXTRACTOR_T(NcoTypeConverter().ToStringOrDefault(i.Primary)),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Primary = NcoTypeConverter().ParseOrDefault(
					va.Read<const char*>(l, si),
					i.Primary
				);

				i.Calculate_Risk(); // make sure the Risk value now reflects the new primary weapon
			},
			ParseCheckValidator<WeaponType>::Build("Weapon", [](auto stringValue) {
				return &NcoTypeConverter().Parse<WeaponType>(stringValue);
			})
		).WithFieldWrapper(
			SECONDARY_WEAPON_RULE,
			EXTRACTOR_T(NcoTypeConverter().ToStringOrDefault(i.Secondary)),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Secondary = NcoTypeConverter().ParseOrDefault(
					va.Read<const char*>(l, si),
					i.Secondary
				);
			},
			ParseCheckValidator<WeaponType>::Build("Weapon", [](auto stringValue) {
				return &NcoTypeConverter().Parse<WeaponType>(stringValue);
			})
		).WithFieldWrapper(
			ARMOR_RULE,
			EXTRACTOR_T(NcoTypeConverter().ToStringOrDefault(i.Armor)),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Armor = NcoTypeConverter().ParseOrDefault(
					va.Read<const char*>(l, si),
					i.Armor
				);
			},
			ParseCheckValidator<ArmorType>::Build("Armor", [](auto stringValue) {
				return &NcoTypeConverter().Parse<ArmorType>(stringValue);
			})
		).WithFieldWrapper(
			AMMO_RULE,
			SIMPLE_EXTRACTOR_T(MaxAmmo),
			SIMPLE_INJECTOR_T(int, MaxAmmo),
			NumbericRangeValidator<>::Build(-1, INT_MAX)
		).WithFieldWrapper(
			SIGHT_RANGE_RULE,
			SIMPLE_EXTRACTOR_T(SightRange),
			SIMPLE_INJECTOR_T(int, SightRange),
			NumbericRangeValidator<>::Build(-1, INT_MAX)
		).WithFieldWrapper(
			REWARD_RULE,
			SIMPLE_EXTRACTOR_T(Reward),
			SIMPLE_INJECTOR_T(int, Reward),
			NumbericRangeValidator<>::Build(-1, INT_MAX)
		).WithFieldWrapper(
			DETECT_CLOAKED_OBJECTS_RULE,
			EXTRACTOR_T((bool)i.IsScanner),
			SIMPLE_INJECTOR_T(bool, IsScanner),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			CRUSHABLE_RULE,
			EXTRACTOR_T((bool)i.IsCrushable),
			SIMPLE_INJECTOR_T(bool, IsCrushable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			LEADER_RULE,
			EXTRACTOR_T((bool)i.IsLeader),
			SIMPLE_INJECTOR_T(bool, IsLeader),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			TWO_SHOOTER_RULE,
			EXTRACTOR_T((bool)i.IsTwoShooter),
			SIMPLE_INJECTOR_T(bool, IsTwoShooter),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_THEATER_GFX_RULE,
			EXTRACTOR_T((bool)i.IsTheater),
			SIMPLE_INJECTOR_T(bool, IsTheater),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			SHOW_NAME_RULE,
			EXTRACTOR_T((bool)i.IsNominal),
			SIMPLE_INJECTOR_T(bool, IsNominal),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			IS_MOD_TYPE_RULE,
			EXTRACTOR_T((bool)i.IsModType),
			SIMPLE_INJECTOR_T(bool, IsModType),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			HAS_CREW_RULE,
			EXTRACTOR_T((bool)i.IsCrew),
			SIMPLE_INJECTOR_T(bool, IsCrew),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			TURRET_EQUIPPED_RULE,
			EXTRACTOR_T((bool)i.IsTurretEquipped),
			SIMPLE_INJECTOR_T(bool, IsTurretEquipped),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			REPAIRABLE_RULE,
			EXTRACTOR_T((bool)i.IsRepairable),
			SIMPLE_INJECTOR_T(bool, IsRepairable),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			TRANSPORTER_RULE,
			EXTRACTOR_T((bool)i.IsTransporter),
			SIMPLE_INJECTOR_T(bool, IsTransporter),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			BASE_TYPE_RULE,
			SIMPLE_EXTRACTOR_T(ModBaseIniName),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				strcpy(i.ModBaseIniName, strdup(va.Read<const char*>(l, si)));
			},
			LambdaValidator<const char*>::Build("String must be at most 32 characters long", [] (const char* v) {
				return StringIsEmpty(v) || strlen(v) < 33;
			})
		).WithFieldWrapper(
			IMAGE_RULE,
			SIMPLE_EXTRACTOR_T(Image),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Image = strdup(va.Read<const char*>(l, si));
			},
			PrimitiveTypeValidator<const char*>::Build()
		).WithFieldWrapper(
			FRIENDLY_NAME_RULE,
			SIMPLE_EXTRACTOR_T(FriendlyName),
			[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.FriendlyName = strdup(va.Read<const char*>(l, si));
			},
			PrimitiveTypeValidator<const char*>::Build()
		);
	}

};
