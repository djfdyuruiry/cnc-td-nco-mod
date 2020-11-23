#pragma once

#include <LambdaValidator.h>
#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <ParseCheckValidator.h>
#include <RulesSectionTypeWrapperApi.h>

#include "../DEFINES.H"
#include "../TYPE.H"

#include "parse.h"
#include "rules_ini_generic.h"
#include "rules_ini_mods.h"
#include "rules_ini_weapon.h"

#define EXTRACTOR_WEAP(f) EXTRACTOR(WeaponTypeClass, f)
#define INJECTOR_WEAP(t, f) INJECTOR(WeaponTypeClass, t, f)
#define SIMPLE_EXTRACTOR_WEAP(f) SIMPLE_EXTRACTOR(WeaponTypeClass, f)
#define SIMPLE_INJECTOR_WEAP(t, f) SIMPLE_INJECTOR(WeaponTypeClass, t, f)

class WeaponApi : public RulesSectionTypeWrapperApi<WeaponTypeClass, WeaponType>
{
protected:
	WeaponApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		RulesSectionTypeWrapperApi(strdup("Weapon"), rulesInfo, WEAPON_FIRST, getCount, ParseWeaponType, WeaponTypeToString)
	{
		technoTypeWrapper.WithFieldWrapper(
			WEAPON_PROJECTILE_RULE,
			EXTRACTOR_WEAP(BulletTypeToString(i.Projectile)),
			[](WeaponTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				auto valueUpper = ConvertStringToUpperCase(va.Read<const char*>(l, si));

				i.Projectile = ParseBulletType(valueUpper, NULL);

				delete valueUpper;
			},
			ParseCheckValidator<BulletType>::Build("Bullet", ParseBulletType)
		).WithFieldWrapper(
			WEAPON_DAMAGE_RULE,
			SIMPLE_EXTRACTOR_WEAP(Attack),
			SIMPLE_INJECTOR_WEAP(unsigned char, Attack),
			NumbericRangeValidator<>::Build(0, UCHAR_MAX)
		).WithFieldWrapper(
			WEAPON_RATE_OF_FIRE_RULE,
			SIMPLE_EXTRACTOR_WEAP(ROF),
			SIMPLE_INJECTOR_WEAP(unsigned char, ROF),
			NumbericRangeValidator<>::Build(0, UCHAR_MAX)
		).WithFieldWrapper(
			WEAPON_RANGE_RULE,
			SIMPLE_EXTRACTOR_WEAP(Range),
			SIMPLE_INJECTOR_WEAP(int, Range),
			NumbericRangeValidator<>::Build(0, INT_MAX)
		).WithFieldWrapper(
			FRIENDLY_NAME_RULE,
			SIMPLE_EXTRACTOR_WEAP(FriendlyName),
			[](WeaponTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.FriendlyName = strdup(va.Read<const char*>(l, si));
			},
			PrimitiveTypeValidator<const char*>::Build()
		).WithFieldWrapper(
			IS_MOD_TYPE_RULE,
			EXTRACTOR_WEAP((bool)i.IsModType),
			SIMPLE_INJECTOR_WEAP(bool, IsModType),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			BASE_TYPE_RULE,
			SIMPLE_EXTRACTOR_WEAP(ModBaseIniName),
			[](WeaponTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				strcpy(i.ModBaseIniName, strdup(va.Read<const char*>(l, si)));
			},
			LambdaValidator<const char*>::Build("String must be at most 32 characters long", [] (const char* v) {
				return StringIsEmpty(v) || strlen(v) < 33;
			})
		);
	}

public:
	static LuaApi& Build(IRulesIniSection& rulesInfo, std::function<int(void)> getCount)
	{
		return *(new WeaponApi(rulesInfo, getCount));
	}

};
