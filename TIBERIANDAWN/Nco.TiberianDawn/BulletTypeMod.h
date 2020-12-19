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
			BULLET_COUNT,
			NcoTypeConverter().GetModTypeMap<BulletType>()
		  )
	{
	}

protected:
	ResultWithValue<BulletType>& ParseType(SectionName typeString)
	{
		return NcoTypeConverter().Parse<BulletType>(typeString);
	}

	void AddRulesSection(SectionName typeString)
	{
		runtime.GetRules() << TiberianDawnRuleSectionBuilder::BuildBulletSection(typeString);
	}

	BulletTypeClass* CloneType(const char* baseTypeString, const char* typeString, BulletType baseType, BulletType type)
	{
		auto newType = TiberianDawnTypeMod::CloneType(baseTypeString, typeString, baseType, type);

		strcpy(newType->RulesName, typeString);

		return newType;
	}

	void ReadRulesAndAddType(BulletTypeClass* type)
	{
		BulletTypeClass::ReadBulletRules(type);

		BulletTypeClass::Add_Bullet_Type(type);
	}

public:
	static BulletTypeMod& Build(IRulesRuntime& runtime)
	{
		return *(new BulletTypeMod(runtime));
	}

};
