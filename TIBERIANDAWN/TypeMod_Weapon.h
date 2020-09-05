#pragma once

#include "function.h"

#include "NcoGameMod.h"

class WeaponTypeMod : public NcoGameMod<WeaponType, WeaponTypeClass>
{
private:
	WeaponTypeMod() 
		: NcoGameMod(
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
		GetRules() << RulesIniSectionBuilder::BuildWeaponSection(typeString);
	}

	void ReadRulesAndAddType(WeaponTypeClass* type)
	{
		WeaponTypeClass::Read_Weapon_Rules(type);

		WeaponTypeClass::Add_Weapon_Type(type);
	}

public:
	static WeaponTypeMod& Build()
	{
		return *(new WeaponTypeMod());
	}

};
