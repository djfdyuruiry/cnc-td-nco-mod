#pragma once

#include "defines.h"

#define WARHEAD_SPREAD_FACTOR_RULE "SpreadFactor"

#define WARHEAD_DESTORY_WALLS_RULE "DestroyWalls"

#define WARHEAD_DESTORY_WOOD_RULE "DestroyWood"

#define WARHEAD_DESTORY_TIBERIUM_RULE "DestroyTiberium"

#define WARHEAD_NO_ARMOR_MODIFIER_RULE "NoArmorModifier"

#define WARHEAD_WOOD_ARMOR_MODIFIER_RULE "WoodArmorModifier"

#define WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE "AluminumArmorModifier"

#define WARHEAD_STEEL_ARMOR_MODIFIER_RULE "SteelArmorModifier"

#define WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE "ConcreteArmorModifier"

const char* Map_Armor_Modifier_To_Warhead_Rule(ArmorType type);
