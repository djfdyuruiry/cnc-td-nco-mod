#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "TiberianDawnTypeMod.h"

class WeaponTypeMod : public TiberianDawnTypeMod<WeaponType, WeaponTypeClass>
{
private:
	WeaponTypeMod(IRulesRuntime& runtime)
		: TiberianDawnTypeMod(
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
		return ParseWeaponType(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildWeaponSection(typeString);
	}

	void ReadRulesAndAddType(WeaponTypeClass* type)
	{
		WeaponTypeClass::ReadWeaponRules(type);

		WeaponTypeClass::Add_Weapon_Type(type);
	}

public:
	static WeaponTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new WeaponTypeMod(runtime));
	}

};
