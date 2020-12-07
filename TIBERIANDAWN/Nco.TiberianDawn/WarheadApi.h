#pragma once

#include <string>

#include <IRulesIniSection.h>
#include <LambdaValidator.h>
#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <ParseCheckValidator.h>
#include <RulesSectionTypeWrapperApi.h>

#include "../DEFINES.H"
#include "../TYPE.H"

#include "rules_ini_generic.h"
#include "rules_ini_mods.h"
#include "rules_ini_warhead.h"
#include "TiberianDawnNcoRuntime.h"

#define EXTRACTOR_WAR(f) EXTRACTOR(WarheadTypeClass, f)
#define INJECTOR_WAR(t, f) INJECTOR(WarheadTypeClass, t, f)
#define SIMPLE_EXTRACTOR_WAR(f) SIMPLE_EXTRACTOR(WarheadTypeClass, f)
#define SIMPLE_INJECTOR_WAR(t, f) SIMPLE_INJECTOR(WarheadTypeClass, t, f)

class WarheadApi : public RulesSectionTypeWrapperApi<WarheadTypeClass, WarheadType>
{
protected:
	WarheadApi(IRulesIniSection& rulesInfo, std::function<int(void)> getCount) :
		RulesSectionTypeWrapperApi(
			strdup("Warhead"),
			rulesInfo,
			WARHEAD_FIRST,
			getCount,
			[](auto typeString) {
				return &NcoTypeConverter().Parse<WarheadType>(typeString);
			},
			[](auto type) {
				return &NcoTypeConverter().ToString(type);
			}
		)
	{
		technoTypeWrapper.WithFieldWrapper(
			WARHEAD_SPREAD_FACTOR_RULE,
			SIMPLE_EXTRACTOR_WAR(SpreadFactor),
			SIMPLE_INJECTOR_WAR(int, SpreadFactor),
			NumbericRangeValidator<>::Build(1, INT_MAX)
		).WithFieldWrapper(
			WARHEAD_DESTORY_WALLS_RULE,
			EXTRACTOR_WAR((bool)i.IsWallDestroyer),
			SIMPLE_INJECTOR_WAR(bool, IsWallDestroyer),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			WARHEAD_DESTORY_WOOD_RULE,
			EXTRACTOR_WAR((bool)i.IsWoodDestroyer),
			SIMPLE_INJECTOR_WAR(bool, IsWoodDestroyer),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			WARHEAD_DESTORY_TIBERIUM_RULE,
			EXTRACTOR_WAR((bool)i.IsTiberiumDestroyer),
			SIMPLE_INJECTOR_WAR(bool, IsTiberiumDestroyer),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			WARHEAD_NO_ARMOR_MODIFIER_RULE,
			EXTRACTOR_WAR(i.Modifier[ARMOR_NONE]),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Modifier[ARMOR_NONE] = va.Read<unsigned int>(l, si);
			},
			NumbericRangeValidator<>::Build(1, UINT_MAX)
		).WithFieldWrapper(
			WARHEAD_WOOD_ARMOR_MODIFIER_RULE,
			EXTRACTOR_WAR(i.Modifier[ARMOR_WOOD]),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Modifier[ARMOR_WOOD] = va.Read<unsigned int>(l, si);
			},
			NumbericRangeValidator<>::Build(1, UINT_MAX)
		).WithFieldWrapper(
			WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE,
			EXTRACTOR_WAR(i.Modifier[ARMOR_ALUMINUM]),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Modifier[ARMOR_ALUMINUM] = va.Read<unsigned int>(l, si);
			},
			NumbericRangeValidator<>::Build(1, UINT_MAX)
		).WithFieldWrapper(
			WARHEAD_STEEL_ARMOR_MODIFIER_RULE,
			EXTRACTOR_WAR(i.Modifier[ARMOR_STEEL]),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Modifier[ARMOR_STEEL] = va.Read<unsigned int>(l, si);
			},
			NumbericRangeValidator<>::Build(1, UINT_MAX)
		).WithFieldWrapper(
			WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE,
			EXTRACTOR_WAR(i.Modifier[ARMOR_CONCRETE]),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.Modifier[ARMOR_CONCRETE] = va.Read<unsigned int>(l, si);
			},
			NumbericRangeValidator<>::Build(1, UINT_MAX)
		).WithFieldWrapper(
			FRIENDLY_NAME_RULE,
			SIMPLE_EXTRACTOR_WAR(FriendlyName),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
				i.FriendlyName = strdup(va.Read<const char*>(l, si));
			},
			PrimitiveTypeValidator<const char*>::Build()
		).WithFieldWrapper(
			IS_MOD_TYPE_RULE,
			EXTRACTOR_WAR((bool)i.IsModType),
			SIMPLE_INJECTOR_WAR(bool, IsModType),
			PrimitiveTypeValidator<bool>::Build()
		).WithFieldWrapper(
			BASE_TYPE_RULE,
			SIMPLE_EXTRACTOR_WAR(ModBaseIniName),
			[](WarheadTypeClass& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
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
		return *(new WarheadApi(rulesInfo, getCount));
	}

};
