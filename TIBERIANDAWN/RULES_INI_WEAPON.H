#ifndef RULES_INI_WEAPON_H
#define RULES_INI_WEAPON_H

#define WEAPON_DAMAGE_RULE "Damage"

#define WEAPON_RATE_OF_FIRE_RULE "RateOfFire"

#define WEAPON_RANGE_RULE "Range"

#define Read_Weapon_Damage(bulletType, defaultValue) (unsigned char) Read_Unsigned_Int_From_Rules_Ini(bulletType, WEAPON_DAMAGE_RULE, defaultValue, 1, UCHAR_MAX)

#define Read_Weapon_Rate_Of_Fire(bulletType, defaultValue) (unsigned char) Read_Unsigned_Int_From_Rules_Ini(bulletType, WEAPON_RATE_OF_FIRE_RULE, defaultValue, 1, UCHAR_MAX)

#define Read_Weapon_Range(bulletType, defaultValue) Read_Int_From_Rules_Ini(bulletType, WEAPON_RANGE_RULE, defaultValue, 1, INT_MAX)

#endif
