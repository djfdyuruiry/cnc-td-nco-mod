#pragma once

#define WEAPON_FIRES_RULE "Fires"
#define WEAPON_FIRES_RULE_UPPER "FIRES"

#define WEAPON_DAMAGE_RULE "Damage"
#define WEAPON_DAMAGE_RULE_UPPER "DAMAGE"

#define WEAPON_RATE_OF_FIRE_RULE "RateOfFire"
#define WEAPON_RATE_OF_FIRE_RULE_UPPER "RATEOFFIRE"

#define WEAPON_RANGE_RULE "Range"
#define WEAPON_RANGE_RULE_UPPER "RANGE"

#define WEAPON_RULES FRIENDLY_NAME_RULE, \
	IS_MOD_TYPE_RULE, \
    WEAPON_FIRES_RULE, \
	WEAPON_DAMAGE_RULE, \
	WEAPON_RATE_OF_FIRE_RULE, \
	WEAPON_RANGE_RULE

#define WEAPON_RULE_COUNT 6u

#define Read_Weapon_Damage(bulletType, defaultValue) (unsigned char) Read_Unsigned_Int_From_Rules_Ini(bulletType, WEAPON_DAMAGE_RULE, defaultValue, 1, UCHAR_MAX)

#define Read_Weapon_Rate_Of_Fire(bulletType, defaultValue) (unsigned char) Read_Unsigned_Int_From_Rules_Ini(bulletType, WEAPON_RATE_OF_FIRE_RULE, defaultValue, 1, UCHAR_MAX)

#define Read_Weapon_Range(bulletType, defaultValue) Read_Int_From_Rules_Ini(bulletType, WEAPON_RANGE_RULE, defaultValue, 1, INT_MAX)

BulletType Read_Bullet_From_Rules_Ini(const char* section, const char* entry, BulletType default);
