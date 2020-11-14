#pragma once

#include <NcoGameMod.h>

#include "function.h"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"

class WeaponTypeMod : public NcoGameMod<WeaponType, WeaponTypeClass>
{
private:
	WeaponTypeMod(IRulesRuntime& runtime)
		: NcoGameMod(
			runtime,
			"Weapon",
			NEW_WEAPONS_RULE,
			NEW_WEAPON_COUNT_RULE,
			*WEAPON_COUNT_RULE_KEY,
			WEAPON_COUNT
		  )
	{
	}

protected:
	WeaponType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Weapon_Type(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWeaponSection(typeString);
	}

	WeaponTypeClass* CloneType(const char* baseTypeString, const char* typeString, WeaponType baseType, WeaponType type)
	{
		auto newType = NcoGameMod::CloneType(baseTypeString, typeString, baseType, type);

		newType->FriendlyName = strdup(newType->FriendlyName);

		return newType;
	}

	void ReadRulesAndAddType(WeaponTypeClass* type)
	{
		WeaponTypeClass::Read_Weapon_Rules(type);

		WeaponTypeClass::Add_Weapon_Type(type);
	}

public:
	static WeaponTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new WeaponTypeMod(runtime));
	}

};
