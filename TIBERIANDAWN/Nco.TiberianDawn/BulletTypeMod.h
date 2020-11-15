#pragma once

#include "../FUNCTION.H"

#include "tiberian_dawn_rule_keys.h"
#include "TiberianDawnRuleSectionBuilder.h"
#include "TiberianDawnTypeMod.h"

class BulletTypeMod : public TiberianDawnTypeMod<BulletType, BulletTypeClass>
{
private:
	BulletTypeMod(IRulesRuntime& runtime) 
		: TiberianDawnTypeMod(
			runtime,
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
		return Parse_Bullet_Type(typeString, parseError, false);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildBulletSection(typeString);
	}

	void ReadRulesAndAddType(BulletTypeClass* type)
	{
		BulletTypeClass::Read_Bullet_Rules(type);

		BulletTypeClass::Add_Bullet_Type(type);
	}

public:
	static BulletTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new BulletTypeMod(runtime));
	}

};
