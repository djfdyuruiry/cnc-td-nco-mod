#pragma once

#include "function.h"

#include "NcoGameMod.h"

class BulletTypeMod : public NcoGameMod<BulletType, BulletTypeClass>
{
private:
	BulletTypeMod() 
		: NcoGameMod(
			"Bullet",
			NEW_BULLETS_RULE,
			NEW_BULLET_COUNT_RULE,
			*BULLET_COUNT_RULE_KEY,
			BULLET_COUNT
		  )
	{
	}

protected:
	BulletType ParseType(SectionName typeString, bool* parseError)
	{
		return Parse_Bullet_Type(typeString, parseError, true);
	}

	void AddRulesSection(SectionName typeString)
	{
		GetRules() << RulesIniSectionBuilder::BuildBulletSection(typeString);
	}

	void ReadRulesAndAddType(BulletTypeClass* type)
	{
		BulletTypeClass::Read_Bullet_Rules(type);

		BulletTypeClass::Add_Bullet_Type(type);
	}

public:
	static BulletTypeMod& Build()
	{
		return *(new BulletTypeMod());
	}

};
