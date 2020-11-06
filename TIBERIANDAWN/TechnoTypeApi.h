#pragma once

#include <map>
#include <vector>
#include <string>

#include <LambdaValidator.h>
#include <LuaValueAdapter.h>
#include <NumericRangeValidator.h>
#include <strings.h>
#include <TypeApi.h>

#include "IRulesIniSection.h"
#include "LuaTypeWrapper.h"
#include "parse.h"
#include "ParseCheckValidator.h"
#include "rules_cache_key.h"
#include "rules_ini_generic.h"
#include "type.h"

#define EXTRACTOR(f) [](T& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(t, f) [](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(f) EXTRACTOR(i.f)
#define SIMPLE_INJECTOR(t, f) INJECTOR(t, i.f) 

template<class T> class TechnoTypeApi : public TypeApi<T>
{
protected:
	IRulesIniSection& rulesInfo;
	LuaTypeWrapper<T>& technoTypeWrapper;

	TechnoTypeApi(const char* typeName, IRulesIniSection& rulesInfo) :
		TypeApi(typeName),
		rulesInfo(rulesInfo),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
	{
		technoTypeWrapper.WithFieldWrapper(
				BUILD_LEVEL_RULE,
				SIMPLE_EXTRACTOR(Level),
				SIMPLE_INJECTOR(unsigned char, Level),
				NumbericRangeValidator<unsigned char>::Build(0, 99)
			).WithFieldWrapper(
				SCENARIO_LEVEL_RULE,
				SIMPLE_EXTRACTOR(Scenario),
				SIMPLE_INJECTOR(unsigned char, Scenario),
				NumbericRangeValidator<unsigned char>::Build(0, 99)
			).WithFieldWrapper(
				PREREQUISITE_RULE,
				EXTRACTOR(Prerequisite_To_String(i.Pre)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Pre = Structure_Type_To_Prerequisite(
						Parse_Structure_Type(va.Read<const char*>(l, si), NULL),
						NULL
					);
				},
				ParseCheckValidator<StructType>::Build(Parse_Structure_Type)
			).WithFieldWrapper(
				COST_RULE,
				SIMPLE_EXTRACTOR(Cost),
				SIMPLE_INJECTOR(int, Cost),
				PrimitiveTypeValidator<int>::Build()
			).WithFieldWrapper(
				BUILDABLE_RULE,
				SIMPLE_EXTRACTOR(IsBuildable),
				SIMPLE_INJECTOR(bool, IsBuildable),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				FLAMMABLE_RULE,
				SIMPLE_EXTRACTOR(IsFlammable),
				SIMPLE_INJECTOR(bool, IsFlammable),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				SPEED_RULE,
				SIMPLE_EXTRACTOR(MaxSpeed),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.MaxSpeed = (MPHType) va.Read<unsigned int>(l, si);
				},
				NumbericRangeValidator<>::Build(0, UCHAR_MAX)
			).WithFieldWrapper(
				STRENGTH_RULE,
				SIMPLE_EXTRACTOR(MaxStrength),
				SIMPLE_INJECTOR(unsigned short, MaxStrength),
				NumbericRangeValidator<>::Build(0, USHRT_MAX)
			).WithFieldsWrapper(
				std::vector<const char*> { OWNER_RULE, HOUSES_RULE },
				SIMPLE_EXTRACTOR(HouseListCsv),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					auto owner = va.Read<const char*>(l, si);

					i.Ownable = Parse_House_Name_List_Csv(owner, NULL);
					i.HouseListCsv = strdup(owner);
				},
				ParseCheckValidator<int>::Build(Parse_House_Name_List_Csv)
			).WithFieldWrapper(
				PRIMARY_WEAPON_RULE,
				EXTRACTOR(Weapon_Type_To_String(i.Primary)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Primary = Parse_Weapon_Type(va.Read<const char*>(l, si), NULL);

					i.Calculate_Risk(); // make sure the Risk value now reflects the new primary weapon
				},
				ParseCheckValidator<WeaponType>::Build(Parse_Weapon_Type)
			).WithFieldWrapper(
				SECONDARY_WEAPON_RULE,
				EXTRACTOR(Weapon_Type_To_String(i.Primary)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Secondary = Parse_Weapon_Type(va.Read<const char*>(l, si), NULL);
				},
				ParseCheckValidator<WeaponType>::Build(Parse_Weapon_Type)
			).WithFieldWrapper(
				ARMOR_RULE,
				EXTRACTOR(Armor_Type_To_String(i.Armor)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Armor = Parse_Armor_Type(va.Read<const char*>(l, si), NULL);
				},
				ParseCheckValidator<ArmorType>::Build(Parse_Armor_Type)
			).WithFieldWrapper(
				AMMO_RULE,
				SIMPLE_EXTRACTOR(MaxAmmo),
				SIMPLE_INJECTOR(int, MaxAmmo),
				NumbericRangeValidator<>::Build(-1, INT_MAX)
			).WithFieldWrapper(
				SIGHT_RANGE_RULE,
				SIMPLE_EXTRACTOR(SightRange),
				SIMPLE_INJECTOR(int, SightRange),
				NumbericRangeValidator<>::Build(-1, INT_MAX)
			).WithFieldWrapper(
				REWARD_RULE,
				SIMPLE_EXTRACTOR(Reward),
				SIMPLE_INJECTOR(int, Reward),
				NumbericRangeValidator<>::Build(-1, INT_MAX)
			).WithFieldWrapper(
				DETECT_CLOAKED_OBJECTS_RULE,
				SIMPLE_EXTRACTOR(IsScanner),
				SIMPLE_INJECTOR(bool, IsScanner),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				CRUSHABLE_RULE,
				SIMPLE_EXTRACTOR(IsCrushable),
				SIMPLE_INJECTOR(bool, IsCrushable),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				LEADER_RULE,
				SIMPLE_EXTRACTOR(IsLeader),
				SIMPLE_INJECTOR(bool, IsLeader),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				TWO_SHOOTER_RULE,
				SIMPLE_EXTRACTOR(IsTwoShooter),
				SIMPLE_INJECTOR(bool, IsTwoShooter),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				HAS_THEATER_GFX_RULE,
				SIMPLE_EXTRACTOR(IsTheater),
				SIMPLE_INJECTOR(bool, IsTheater),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				SHOW_NAME_RULE,
				SIMPLE_EXTRACTOR(IsNominal),
				SIMPLE_INJECTOR(bool, IsNominal),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				IS_MOD_TYPE_RULE,
				SIMPLE_EXTRACTOR(IsModType),
				SIMPLE_INJECTOR(bool, IsModType),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				HAS_CREW_RULE,
				SIMPLE_EXTRACTOR(IsCrew),
				SIMPLE_INJECTOR(bool, IsCrew),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				TURRET_EQUIPPED_RULE,
				SIMPLE_EXTRACTOR(IsTurretEquipped),
				SIMPLE_INJECTOR(bool, IsTurretEquipped),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				REPAIRABLE_RULE,
				SIMPLE_EXTRACTOR(IsRepairable),
				SIMPLE_INJECTOR(bool, IsRepairable),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				TRANSPORTER_RULE,
				SIMPLE_EXTRACTOR(IsTransporter),
				SIMPLE_INJECTOR(bool, IsTransporter),
				PrimitiveTypeValidator<bool>::Build()
			).WithFieldWrapper(
				BASE_TYPE_RULE,
				SIMPLE_EXTRACTOR(ModBaseIniName),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					strcpy(
						i.ModBaseIniName,
						strdup(va.Read<const char*>(l, si))
					);
				},
				LambdaValidator<const char*>::Build([] (const char* v) {
					return !String_Is_Empty(v) && strlen(v) < 33;
				})
			).WithFieldWrapper(
				IMAGE_RULE,
				SIMPLE_EXTRACTOR(Image),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.Image = strdup(va.Read<const char*>(l, si));
				},
				PrimitiveTypeValidator<const char*>::Build()
			).WithFieldWrapper(
				FRIENDLY_NAME_RULE,
				SIMPLE_EXTRACTOR(FriendlyName),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					i.FriendlyName = strdup(va.Read<const char*>(l, si));
				},
				PrimitiveTypeValidator<const char*>::Build()
			);
	}

	bool ValidateRule(const char* ruleName)
	{
		auto ruleKey = rulesInfo.BuildKey(ruleName);

		return rulesInfo.HasRule(ruleKey);
	}

	bool ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName)
	{
		if (!ValidateRule(ruleName))
		{
			return false;
		}

		technoTypeWrapper.ReadFieldValue(typeInstance, Build_Rule_Key(ruleName), lua);

		return true;
	}

	bool WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex)
	{
		if (!ValidateRule(ruleName))
		{
			return false;
		}

		return technoTypeWrapper.WriteFieldValue(typeInstance, Build_Rule_Key(ruleName), lua, ruleValueStackIndex);
	}

public:
	~TechnoTypeApi()
	{
		delete &technoTypeWrapper;
	}

};
