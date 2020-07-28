#pragma once

#include "rules_ini.h"

#define WEAPON_FIRES_RULE "Fires"

#define WEAPON_DAMAGE_RULE "Damage"

#define WEAPON_RATE_OF_FIRE_RULE "RateOfFire"

#define WEAPON_RANGE_RULE "Range"

#define Read_Weapon_Damage(bulletType, defaultValue) (unsigned char) ReadRuleValue<unsigned int>(bulletType, WEAPON_DAMAGE_RULE, defaultValue)

#define Read_Weapon_Rate_Of_Fire(bulletType, defaultValue) (unsigned char) ReadRuleValue<unsigned int>(bulletType, WEAPON_RATE_OF_FIRE_RULE, defaultValue)

#define Read_Weapon_Range(bulletType, defaultValue) ReadRuleValue<unsigned int>(bulletType, WEAPON_RANGE_RULE, defaultValue)
