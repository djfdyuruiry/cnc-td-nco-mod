#include "rules_ini_warhead.h"

static const char** ARMOR_MODIFIER_RULES = new const char* [ARMOR_COUNT] {
	WARHEAD_NO_ARMOR_MODIFIER_RULE,
	WARHEAD_WOOD_ARMOR_MODIFIER_RULE,
	WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE,
	WARHEAD_STEEL_ARMOR_MODIFIER_RULE,
	WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE
};

const char* Map_Armor_Modifier_To_Warhead_Rule(ArmorType type)
{
	return ARMOR_MODIFIER_RULES[type];
}
