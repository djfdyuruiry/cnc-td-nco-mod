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

#define EXTRACTOR(it, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va) { va.Write(l, f); }
#define INJECTOR(it, t, f) [](it& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) { f = va.Read<t>(l, si); } 
#define SIMPLE_EXTRACTOR(it, f) EXTRACTOR(it, i.f)
#define SIMPLE_INJECTOR(it, t, f) INJECTOR(it, t, i.f) 

#define EXTRACTOR_T(f) EXTRACTOR(T, f)
#define INJECTOR_T(t, f) INJECTOR(T, t, f)
#define SIMPLE_EXTRACTOR_T(f) SIMPLE_EXTRACTOR(T, f)
#define SIMPLE_INJECTOR_T(t, f) SIMPLE_INJECTOR(T, t, f)

template<class T, class U> class TechnoTypeApi : public TypeApi<T>
{
protected:
	IRulesIniSection& rulesInfo;
	LuaTypeWrapper<T>& technoTypeWrapper;
	U(*typeParser)(const char*, bool*, bool);

	TechnoTypeApi(const char* typeName, IRulesIniSection& rulesInfo, U(*typeParser)(const char*, bool*, bool)) :
		TypeApi(typeName),
		rulesInfo(rulesInfo),
		typeParser(typeParser),
		technoTypeWrapper(LuaTypeWrapper<T>::Build())
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
				EXTRACTOR_T(Prerequisite_To_String(i.Pre)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

					i.Pre = Structure_Type_To_Prerequisite(
						Parse_Structure_Type(valueUpper, NULL),
						NULL
					);

					delete valueUpper;
				},
				ParseCheckValidator<StructType>::Build(Parse_Structure_Type)
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
					auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

					i.Ownable = Parse_House_Name_List_Csv(valueUpper, NULL);
					i.HouseListCsv = strdup(valueUpper);

					delete valueUpper;
				},
				ParseCheckValidator<int>::Build(Parse_House_Name_List_Csv)
			).WithFieldWrapper(
				PRIMARY_WEAPON_RULE,
				EXTRACTOR_T(Weapon_Type_To_String(i.Primary)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

					i.Primary = Parse_Weapon_Type(valueUpper, NULL);

					delete valueUpper;

					i.Calculate_Risk(); // make sure the Risk value now reflects the new primary weapon+
				},
				ParseCheckValidator<WeaponType>::Build(Parse_Weapon_Type)
			).WithFieldWrapper(
				SECONDARY_WEAPON_RULE,
				EXTRACTOR_T(Weapon_Type_To_String(i.Secondary)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

					i.Secondary = Parse_Weapon_Type(valueUpper, NULL);

					delete valueUpper;
				},
				ParseCheckValidator<WeaponType>::Build(Parse_Weapon_Type)
			).WithFieldWrapper(
				ARMOR_RULE,
				EXTRACTOR_T(Armor_Type_To_String(i.Armor)),
				[](T& i, ILuaStateWrapper& l, LuaValueAdapter& va, int si) {
					auto valueUpper = Convert_String_To_Upper_Case(va.Read<const char*>(l, si));

					i.Armor = Parse_Armor_Type(valueUpper, NULL);

					delete valueUpper;
				},
				ParseCheckValidator<ArmorType>::Build(Parse_Armor_Type)
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
					strcpy(
						i.ModBaseIniName,
						strdup(va.Read<const char*>(l, si))
					);
				},
				LambdaValidator<const char*>::Build([] (const char* v) {
					return String_Is_Empty(v) || strlen(v) < 33;
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

	bool ValidateTypeName(const char* name)
	{
		bool parseError = false;

		typeParser(name, &parseError, false);

		return !parseError;
	}

	T& ParseType(const char* name)
	{
		return (T&)T::As_Reference(
			typeParser(name, NULL, false)
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