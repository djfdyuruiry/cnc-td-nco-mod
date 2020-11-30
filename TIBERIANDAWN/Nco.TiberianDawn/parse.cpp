#include <strings.h>
#include <utils.h>

#include "../FUNCTION.H"

#include "tiberian_dawn_rules.h"
#include "TiberianDawnRulesInfo.h"

static const auto HOUSE_NAME_MAX_LENGTH = 8;
static const auto INFANTRY_TYPE_MAP = new const char* [INFANTRY_RAMBO] {
    "E1",
    "E2",
    "E3",
    "E4",
    "E5",
    "E6"
};

static const auto CIVILIAN_TYPE_MAP = new const char* [INFANTRY_C10 - INFANTRY_C1] {
    "C1",
    "C2",
    "C3",
    "C4",
    "C5",
    "C6",
    "C7",
    "C8",
    "C9"
};

DiffType ParseDifficultyType(char* difficultyTypeString, bool* parseError, bool ignoreModTypes)
{
    DiffType difficultyType;

    if (StringsAreEqual(difficultyTypeString, "EASY"))
    {
        difficultyType = DIFF_EASY;
    }
    else if (StringsAreEqual(difficultyTypeString, "NORMAL"))
    {
        difficultyType = DIFF_NORMAL;
    }
    else if (StringsAreEqual(difficultyTypeString, "HARD"))
    {
        difficultyType = DIFF_HARD;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        ShowError("Unable to parse difficulty from string: %s", difficultyTypeString);
    }

    return difficultyType;
}

const char* DifficultyTypeToString(DiffType difficultyType)
{
    const char* difficultyTypeString;

    if (difficultyType == DIFF_EASY)
    {
        difficultyTypeString = "EASY";
    }
    else if (difficultyType == DIFF_NORMAL)
    {
        difficultyTypeString = "NORMAL";
    }
    else if (difficultyType == DIFF_HARD)
    {
        difficultyTypeString = "HARD";
    }
    else
    {
        ShowError("Unable to convert difficulty type to string: %d", difficultyType);
    }

    return difficultyTypeString;
}

HousesType ParseHouseType(char* houseTypeString, bool* parseError, bool ignoreModTypes)
{
    HousesType house;

    if (StringsAreEqual(houseTypeString, "NONE"))
    {
        house = HOUSE_NONE;
    }
    else if (StringsAreEqual(houseTypeString, "GOODGUY"))
    {
        house = HOUSE_GOOD;
    }
    else if (StringsAreEqual(houseTypeString, "BADGUY"))
	{
		house = HOUSE_BAD;
	}
	else if (StringsAreEqual(houseTypeString, "NEUTRAL"))
	{
		house = HOUSE_NEUTRAL;
	}
	else if (StringsAreEqual(houseTypeString, "JP"))
	{
		house = HOUSE_JP;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI1"))
	{
		house = HOUSE_MULTI1;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI2"))
	{
        house = HOUSE_MULTI2;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI3"))
	{
        house = HOUSE_MULTI3;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI4"))
	{
        house = HOUSE_MULTI4;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI5"))
	{
        house = HOUSE_MULTI5;
	}
	else if (StringsAreEqual(houseTypeString, "MULTI6"))
	{
        house = HOUSE_MULTI6;
	}
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        ShowError("Unable to parse house from string: %s", houseTypeString);
    }

	return house;
}

HousesType ParseHouseType(const char* houseTypeString, bool* parseError, bool ignoreModTypes)
{
    auto houseTypeStr = strdup(houseTypeString);

    auto houseType = ParseHouseType(houseTypeStr, parseError, ignoreModTypes);

    delete houseTypeStr;

    return houseType;
}

int ParseHouseNameListCsv(char* houseListCsv, bool* parseError, bool ignoreModTypes)
{
    auto houseNameListSize = 0u;
    auto houseNameList = ParseCsvString(houseListCsv, HOUSE_NAME_MAX_LENGTH, &houseNameListSize);

    if (houseNameList == NULL)
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        return HOUSEF_NONE;
    }

    auto houseListInitialised = false;
    auto houseList = 0;

    for (auto i = 0u; i < houseNameListSize; i++)
    {
        bool houseParseError = false;
        auto house = ParseHouseType(houseNameList[i], &houseParseError, ignoreModTypes);

        if (houseParseError)
        {
            if (parseError != NULL)
            {
                *parseError = true;
            }

            // unable to parse entry as a house name
            return HOUSEF_NONE;
        }

        if (house == HOUSE_NONE)
        {
            // none overrides all entries in csv
            return HOUSEF_NONE;
        }

        auto houseBit = 1 << house;

        if (!houseListInitialised)
        {
            houseList = houseBit;
            houseListInitialised = true;
        }
        else
        {
            houseList = houseList | houseBit;
        }
    }

    delete houseNameList;

    return houseList;
}

int ParseHouseNameListCsv(const char* houseListCsv, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(houseListCsv))
    {
        ShowError("CSV passed to ParseHouseNameListCsv was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return HOUSEF_NONE;
    }

    auto houseTypeCsvStr = strdup(houseListCsv);

    auto owner = ParseHouseNameListCsv(houseTypeCsvStr, parseError, ignoreModTypes);

    delete houseTypeCsvStr;

    return owner;
}

const char* HouseTypeToString(HousesType houseType)
{
    const char* houseTypeString;

    if (houseType == HOUSE_NONE)
    {
        houseTypeString, "NONE";
    }
    else if (houseType == HOUSE_GOOD)
    {
        houseTypeString = "GOODGUY";
    }
    else if (houseType == HOUSE_BAD)
    {
        houseTypeString = "BADGUY";
    }
    else if (houseType == HOUSE_NEUTRAL)
    {
        houseTypeString = "NEUTRAL";
    }
    else if (houseType == HOUSE_JP)
    {
        houseTypeString = "JP";
    }
    else if (houseType == HOUSE_MULTI1)
    {
        houseTypeString = "MULTI1";
    }
    else if (houseType == HOUSE_MULTI2)
    {
        houseTypeString = "MULTI2";
    }
    else if (houseType == HOUSE_MULTI3)
    {
        houseTypeString = "MULTI3";
    }
    else if (houseType == HOUSE_MULTI4)
    {
        houseTypeString = "MULTI4";
    }
    else if (houseType == HOUSE_MULTI5)
    {
        houseTypeString = "MULTI5";
    }
    else if (houseType == HOUSE_MULTI6)
    {
        houseTypeString = "MULTI6";
    }
    else
    {
        ShowError("Unable to convert house type to string: %d", houseType);
    }

    return houseTypeString;
}

StructType PrerequisiteToStructureType(long prerequisite)
{
    auto structType = STRUCT_NONE;
    
    if (prerequisite == STRUCTF_NONE || prerequisite < 0l)
    {
        return STRUCT_NONE;
    }
    else if (prerequisite == STRUCTF_ADVANCED_POWER)
    {
        return STRUCT_ADVANCED_POWER;
    }
    else if (prerequisite == STRUCTF_REPAIR)
    {
        return STRUCT_REPAIR;
    }
    else if (prerequisite == STRUCTF_EYE)
    {
        return STRUCT_EYE;
    }
    else if (prerequisite == STRUCTF_TEMPLE)
    {
        return STRUCT_TEMPLE;
    }
    else if (prerequisite == STRUCTF_HAND)
    {
        return STRUCT_HAND;
    }
    else if (prerequisite == STRUCTF_BIO_LAB)
    {
        return STRUCT_BIO_LAB;
    }
    else if (prerequisite == STRUCTF_OBELISK)
    {
        return STRUCT_OBELISK;
    }
    else if (prerequisite == STRUCTF_ATOWER)
    {
        return STRUCT_ATOWER;
    }
    else if (prerequisite == STRUCTF_WEAP)
    {
        return STRUCT_WEAP;
    }
    else if (prerequisite == STRUCTF_GTOWER)
    {
        return STRUCT_GTOWER;
    }
    else if (prerequisite == STRUCTF_RADAR)
    {
        return STRUCT_RADAR;
    }
    else if (prerequisite == STRUCTF_TURRET)
    {
        return STRUCT_TURRET;
    }
    else if (prerequisite == STRUCTF_CONST)
    {
        return STRUCT_CONST;
    }
    else if (prerequisite == STRUCTF_REFINERY)
    {
        return STRUCT_REFINERY;
    }
    else if (prerequisite == STRUCTF_STORAGE)
    {
        return STRUCT_STORAGE;
    }
    else if (prerequisite == STRUCTF_HELIPAD)
    {
        return STRUCT_HELIPAD;
    }
    else if (prerequisite == STRUCTF_SAM)
    {
        return STRUCT_SAM;
    }
    else if (prerequisite == STRUCTF_AIRSTRIP)
    {
        return STRUCT_AIRSTRIP;
    }
    else if (prerequisite == STRUCTF_POWER)
    {
        return STRUCT_POWER;
    }
    else if (prerequisite == STRUCTF_HOSPITAL)
    {
        return STRUCT_HOSPITAL;
    }
    else if (prerequisite == STRUCTF_BARRACKS)
    {
        return STRUCT_BARRACKS;
    }
    else if (prerequisite == STRUCTF_TANKER)
    {
        return STRUCT_TANKER;
    }
    else if (prerequisite == STRUCTF_MISSION)
    {
        return STRUCT_MISSION;
    }
    else
    {
        ShowError("Unable to convert prerequiste to structure type: %d", structType);
    }

    return structType;
}

const char* PrerequisiteToString(long prerequisite)
{
    return StructureTypeToString(
        PrerequisiteToStructureType(prerequisite)
    );
}

WeaponType ParseWeaponType(char* weaponTypeString, bool* parseError, bool ignoreModTypes)
{
    WeaponType weaponType = WEAPON_NONE;

    if (StringsAreEqual(weaponTypeString, "NONE"))
    {
        return WEAPON_NONE;
    }
    else if (StringsAreEqual(weaponTypeString, "RIFLE"))
    {
        return WEAPON_RIFLE;
    }
    else if (StringsAreEqual(weaponTypeString, "CHAIN_GUN"))
    {
        return WEAPON_CHAIN_GUN;
    }
    else if (StringsAreEqual(weaponTypeString, "PISTOL"))
    {
        return WEAPON_PISTOL;
    }
    else if (StringsAreEqual(weaponTypeString, "M16"))
    {
        return WEAPON_M16;
    }
    else if (StringsAreEqual(weaponTypeString, "DRAGON"))
    {
        return WEAPON_DRAGON;
    }
    else if (StringsAreEqual(weaponTypeString, "FLAMETHROWER"))
    {
        return WEAPON_FLAMETHROWER;
    }
    else if (StringsAreEqual(weaponTypeString, "FLAME_TONGUE"))
    {
        return WEAPON_FLAME_TONGUE;
    }
    else if (StringsAreEqual(weaponTypeString, "CHEMSPRAY"))
    {
        return WEAPON_CHEMSPRAY;
    }
    else if (StringsAreEqual(weaponTypeString, "GRENADE"))
    {
        return WEAPON_GRENADE;
    }
    else if (StringsAreEqual(weaponTypeString, "75MM"))
    {
        return WEAPON_75MM;
    }
    else if (StringsAreEqual(weaponTypeString, "105MM"))
    {
        return WEAPON_105MM;
    }
    else if (StringsAreEqual(weaponTypeString, "120MM"))
    {
        return WEAPON_120MM;
    }
    else if (StringsAreEqual(weaponTypeString, "TURRET_GUN"))
    {
        return WEAPON_TURRET_GUN;
    }
    else if (StringsAreEqual(weaponTypeString, "MAMMOTH_TUSK"))
    {
        return WEAPON_MAMMOTH_TUSK;
    }
    else if (StringsAreEqual(weaponTypeString, "ROCKET_LAUNCHER"))
    {
        return WEAPON_MLRS;
    }
    else if (StringsAreEqual(weaponTypeString, "155MM"))
    {
        return WEAPON_155MM;
    }
    else if (StringsAreEqual(weaponTypeString, "M60MG"))
    {
        return WEAPON_M60MG;
    }
    else if (StringsAreEqual(weaponTypeString, "TOMAHAWK"))
    {
        return WEAPON_TOMAHAWK;
    }
    else if (StringsAreEqual(weaponTypeString, "TOWTWO"))
    {
        return WEAPON_TOW_TWO;
    }
    else if (StringsAreEqual(weaponTypeString, "NAPALM"))
    {
        return WEAPON_NAPALM;
    }
    else if (StringsAreEqual(weaponTypeString, "OBELISK_LASER"))
    {
        return WEAPON_OBELISK_LASER;
    }
    else if (StringsAreEqual(weaponTypeString, "NIKE"))
    {
        return WEAPON_NIKE;
    }
    else if (StringsAreEqual(weaponTypeString, "HONEST_JOHN"))
    {
        return WEAPON_HONEST_JOHN;
    }
    else if (StringsAreEqual(weaponTypeString, "DINO_RAM"))
    {
        return WEAPON_STEG;
    }
    else if (StringsAreEqual(weaponTypeString, "DINO_BITE"))
    {
        return WEAPON_TREX;
    }


    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newWeaponType = GetNewWeaponType(weaponTypeString, &matchFound);

        if (matchFound)
        {
            return (WeaponType)newWeaponType;
        }
    }

    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse weapon type from string: %s", weaponTypeString);

    return weaponType;
}

WeaponType ParseWeaponType(const char* weaponTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(weaponTypeString))
    {
        ShowError("Weapon type passed to ParseWeaponType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return WEAPON_NONE;
    }

    auto weaponTypeStr = strdup(weaponTypeString);

    auto weaponType = ParseWeaponType(weaponTypeStr, parseError, ignoreModTypes);

    delete weaponTypeStr;

    return weaponType;
}

const char* WeaponTypeToString(WeaponType weaponType, bool ignoreModTypes)
{
    const char* weaponTypeString;

    if (weaponType == WEAPON_NONE)
    {
        weaponTypeString = "NONE";
    }
    else if (weaponType == WEAPON_RIFLE)
    {
        weaponTypeString = "RIFLE";
    }
    else if (weaponType == WEAPON_CHAIN_GUN)
    {
        weaponTypeString = "CHAIN_GUN";
    }
    else if (weaponType == WEAPON_PISTOL)
    {
        weaponTypeString = "PISTOL";
    }
    else if (weaponType == WEAPON_M16)
    {
        weaponTypeString = "M16";
    }
    else if (weaponType == WEAPON_DRAGON)
    {
        weaponTypeString = "DRAGON";
    }
    else if (weaponType == WEAPON_FLAMETHROWER)
    {
        weaponTypeString = "FLAMETHROWER";
    }
    else if (weaponType == WEAPON_FLAME_TONGUE)
    {
        weaponTypeString = "FLAME_TONGUE";
    }
    else if (weaponType == WEAPON_CHEMSPRAY)
    {
        weaponTypeString = "CHEMSPRAY";
    }
    else if (weaponType == WEAPON_GRENADE)
    {
        weaponTypeString = "GRENADE";
    }
    else if (weaponType == WEAPON_75MM)
    {
        weaponTypeString = "75MM";
    }
    else if (weaponType == WEAPON_105MM)
    {
        weaponTypeString = "105MM";
    }
    else if (weaponType == WEAPON_120MM)
    {
        weaponTypeString = "120MM";
    }
    else if (weaponType == WEAPON_TURRET_GUN)
    {
        weaponTypeString = "TURRET_GUN";
    }
    else if (weaponType == WEAPON_MAMMOTH_TUSK)
    {
        weaponTypeString = "MAMMOTH_TUSK";
    }
    else if (weaponType == WEAPON_MLRS)
    {
        weaponTypeString = "ROCKET_LAUNCHER";
    }
    else if (weaponType == WEAPON_155MM)
    {
        weaponTypeString = "155MM";
    }
    else if (weaponType == WEAPON_M60MG)
    {
        weaponTypeString = "M60MG";
    }
    else if (weaponType == WEAPON_TOMAHAWK)
    {
        weaponTypeString = "TOMAHAWK";
    }
    else if (weaponType == WEAPON_TOW_TWO)
    {
        weaponTypeString = "TOWTWO";
    }
    else if (weaponType == WEAPON_NAPALM)
    {
        weaponTypeString = "NAPALM";
    }
    else if (weaponType == WEAPON_OBELISK_LASER)
    {
        weaponTypeString = "OBELISK_LASER";
    }
    else if (weaponType == WEAPON_NIKE)
    {
        weaponTypeString = "NIKE";
    }
    else if (weaponType == WEAPON_HONEST_JOHN)
    {
        weaponTypeString = "HONEST_JOHN";
    }
    else if (weaponType == WEAPON_STEG)
    {
        weaponTypeString = "DINO_RAM";
    }
    else if (weaponType == WEAPON_TREX)
    {
        weaponTypeString = "DINO_BITE";
    }
    else if (!ignoreModTypes && weaponType < ReadWeaponCount())
    {
        weaponTypeString = GetNewWeaponIniName(weaponType);
    }
    else
    {
        ShowError("Unable to convert weapon type to string: %d", weaponType);
    }

    return weaponTypeString;
}

ArmorType ParseArmorType(char* armorTypeString, bool* parseError, bool ignoreModTypes)
{
    ArmorType armor;

    if (StringsAreEqual(armorTypeString, "NONE"))
    {
        armor = ARMOR_NONE;
    }
    else if (StringsAreEqual(armorTypeString, "WOOD"))
    {
        armor = ARMOR_WOOD;
    }
    else if (StringsAreEqual(armorTypeString, "ALUMINUM"))
    {
        armor = ARMOR_ALUMINUM;
    }
    else if (StringsAreEqual(armorTypeString, "STEEL"))
    {
        armor = ARMOR_STEEL;
    }
    else if (StringsAreEqual(armorTypeString, "CONCRETE"))
    {
        armor = ARMOR_CONCRETE;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        ShowError("Unable to parse armor type from string: %s", armorTypeString);
    }

    return armor;
}

ArmorType ParseArmorType(const char* armorTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(armorTypeString))
    {
        ShowError("Armor type passed to ParseArmorType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return ARMOR_NONE;
    }

    auto armorTypeStr = strdup(armorTypeString);

    auto armorType = ParseArmorType(armorTypeStr, parseError, ignoreModTypes);

    delete armorTypeStr;

    return armorType;
}

const char* ArmorTypeToString(ArmorType armorType)
{
    const char* armorTypeString;

    if (armorType == ARMOR_NONE)
    {
        armorTypeString = "NONE";
    }
    else if (armorType == ARMOR_WOOD)
    {
        armorTypeString = "WOOD";
    }
    else if (armorType == ARMOR_ALUMINUM)
    {
        armorTypeString = "ALUMINUM";
    }
    else if (armorType == ARMOR_STEEL)
    {
        armorTypeString = "STEEL";
    }
    else if (armorType == ARMOR_CONCRETE)
    {
        armorTypeString = "CONCRETE";
    }
    else
    {
        ShowError("Unable to convert armor type to string: %u", armorType);
    }

    return armorTypeString;
}

BulletType ParseBulletType(char* bulletTypeString, bool* parseError, bool ignoreModTypes)
{
    BulletType bullet = BULLET_NONE;

    if (StringsAreEqual(bulletTypeString, "NONE"))
    {
        return BULLET_NONE;
    }
    else if (StringsAreEqual(bulletTypeString, "SNIPER"))
    {
        return BULLET_SNIPER;
    }
    else if (StringsAreEqual(bulletTypeString, "BULLET"))
    {
        return BULLET_BULLET;
    }
    else if (StringsAreEqual(bulletTypeString, "SPREADFIRE"))
    {
        return BULLET_SPREADFIRE;
    }
    else if (StringsAreEqual(bulletTypeString, "APDS"))
    {
        return BULLET_APDS;
    }
    else if (StringsAreEqual(bulletTypeString, "ARTILLERY_SHELL"))
    {
        return BULLET_HE;
    }
    else if (StringsAreEqual(bulletTypeString, "SS_MISSILE"))
    {
        return BULLET_SSM;
    }
    else if (StringsAreEqual(bulletTypeString, "MLRS_ROCKET"))
    {
        return BULLET_SSM2;
    }
    else if (StringsAreEqual(bulletTypeString, "SAM_MISSILE"))
    {
        return BULLET_SAM;
    }
    else if (StringsAreEqual(bulletTypeString, "TOW_MISSILE"))
    {
        return BULLET_TOW;
    }
    else if (StringsAreEqual(bulletTypeString, "FLAME"))
    {
        return BULLET_FLAME;
    }
    else if (StringsAreEqual(bulletTypeString, "CHEMICALS"))
    {
        return BULLET_CHEMSPRAY;
    }
    else if (StringsAreEqual(bulletTypeString, "NAPALM_BOMB"))
    {
        return BULLET_NAPALM;
    }
    else if (StringsAreEqual(bulletTypeString, "GRENADE_BOMB"))
    {
        return BULLET_GRENADE;
    }
    else if (StringsAreEqual(bulletTypeString, "LASER_BEAM"))
    {
        return BULLET_LASER;
    }
    else if (StringsAreEqual(bulletTypeString, "NUKE_UP"))
    {
        return BULLET_NUKE_UP;
    }
    else if (StringsAreEqual(bulletTypeString, "NUKE_DOWN"))
    {
        return BULLET_NUKE_DOWN;
    }
    else if (StringsAreEqual(bulletTypeString, "HONEST_JOHN_MISSILE"))
    {
        return BULLET_HONEST_JOHN;
    }
    else if (StringsAreEqual(bulletTypeString, "GORE"))
    {
        return BULLET_HEADBUTT;
    }
    else if (StringsAreEqual(bulletTypeString, "CHEW"))
    {
        return BULLET_TREXBITE;
    }
    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newBulletType = GetNewBulletType(bulletTypeString, &matchFound);

        if (matchFound)
        {
            return (BulletType)newBulletType;
        }
    }
    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse bullet type from string: %s", bulletTypeString);

    return bullet;
}

BulletType ParseBulletType(const char* bulletTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(bulletTypeString))
    {
        ShowError("Bullet type passed to ParseBulletType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return BULLET_NONE;
    }

    auto bulletTypeStr = strdup(bulletTypeString);

    auto bulletType = ParseBulletType(bulletTypeStr, parseError, ignoreModTypes);

    delete bulletTypeStr;

    return bulletType;
}

const char* BulletTypeToString(BulletType bulletType, bool ignoreModTypes)
{
    const char* bulletTypeString;

    if (bulletType == BULLET_NONE)
    {
        bulletTypeString = "NONE";
    }
    else if (bulletType == BULLET_SNIPER)
    {
        bulletTypeString = "SNIPER";
    }
    else if (bulletType == BULLET_BULLET)
    {
        bulletTypeString = "BULLET";
    }
    else if (bulletType == BULLET_SPREADFIRE)
    {
        bulletTypeString = "SPREADFIRE";
    }
    else if (bulletType == BULLET_APDS)
    {
        bulletTypeString = "APDS";
    }
    else if (bulletType == BULLET_HE)
    {
        bulletTypeString = "ARTILLERY_SHELL";
    }
    else if (bulletType == BULLET_SSM)
    {
        bulletTypeString = "SS_MISSILE";
    }
    else if (bulletType == BULLET_SSM2)
    {
        bulletTypeString = "MLRS_ROCKET";
    }
    else if (bulletType == BULLET_SAM)
    {
        bulletTypeString = "SAM_MISSILE";
    }
    else if (bulletType == BULLET_TOW)
    {
        bulletTypeString = "TOW_MISSILE";
    }
    else if (bulletType == BULLET_FLAME)
    {
        bulletTypeString = "FLAME";
    }
    else if (bulletType == BULLET_CHEMSPRAY)
    {
        bulletTypeString = "CHEMICALS";
    }
    else if (bulletType == BULLET_NAPALM)
    {
        bulletTypeString = "NAPALM_BOMB";
    }
    else if (bulletType == BULLET_GRENADE)
    {
        bulletTypeString = "GRENADE_BOMB";
    }
    else if (bulletType == BULLET_LASER)
    {
        bulletTypeString = "LASER_BEAM";
    }
    else if (bulletType == BULLET_NUKE_UP)
    {
        bulletTypeString = "NUKE_UP";
    }
    else if (bulletType == BULLET_NUKE_DOWN)
    {
        bulletTypeString = "NUKE_DOWN";
    }
    else if (bulletType == BULLET_HONEST_JOHN)
    {
        bulletTypeString = "HONEST_JOHN_MISSILE";
    }
    else if (bulletType == BULLET_HEADBUTT)
    {
        bulletTypeString = "GORE";
    }
    else if (bulletType == BULLET_TREXBITE)
    {
        bulletTypeString = "CHEW";
    }
    else if (!ignoreModTypes && bulletType < ReadBulletCount())
    {
        bulletTypeString = GetNewBulletIniName(bulletType);
    }
    else
    {
        ShowError("Unable to convert bullet type to string: %d", bulletType);
    }

    return bulletTypeString;
}

WarheadType ParseWarheadType(char* warheadTypeString, bool* parseError, bool ignoreModTypes)
{
    WarheadType warhead = WARHEAD_NONE;

    if (StringsAreEqual(warheadTypeString, "SA"))
    {
        return WARHEAD_SA;
    }
    else if (StringsAreEqual(warheadTypeString, "HE"))
    {
        return WARHEAD_HE;
    }
    else if (StringsAreEqual(warheadTypeString, "AP"))
    {
        return WARHEAD_AP;
    }
    else if (StringsAreEqual(warheadTypeString, "FIRE"))
    {
        return WARHEAD_FIRE;
    }
    else if (StringsAreEqual(warheadTypeString, "LASER"))
    {
        return WARHEAD_LASER;
    }
    else if (StringsAreEqual(warheadTypeString, "PB"))
    {
        return WARHEAD_PB;
    }
    else if (StringsAreEqual(warheadTypeString, "FIST"))
    {
        return WARHEAD_FIST;
    }
    else if (StringsAreEqual(warheadTypeString, "FOOT"))
    {
        return WARHEAD_FOOT;
    }
    else if (StringsAreEqual(warheadTypeString, "HOLLOW_POINT"))
    {
        return WARHEAD_HOLLOW_POINT;
    }
    else if (StringsAreEqual(warheadTypeString, "SPORE"))
    {
        return WARHEAD_SPORE;
    }
    else if (StringsAreEqual(warheadTypeString, "HEADBUTT"))
    {
        return WARHEAD_HEADBUTT;
    }
    else if (StringsAreEqual(warheadTypeString, "FEEDME"))
    {
        return WARHEAD_FEEDME;
    }
    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newWarheadType = GetNewWarheadType(warheadTypeString, &matchFound);

        if (matchFound)
        {
            return (WarheadType)newWarheadType;
        }
    }

    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse warhead type from string: %s", warheadTypeString);

    return warhead;
}

WarheadType ParseWarheadType(const char* warheadTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(warheadTypeString))
    {
        ShowError("Warhead type passed to ParseWarheadType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return WARHEAD_NONE;
    }

    auto warheadTypeStr = strdup(warheadTypeString);

    auto warheadType = ParseWarheadType(warheadTypeStr, parseError, ignoreModTypes);

    delete warheadTypeStr;

    return warheadType;
}

const char* WarheadTypeToString(WarheadType warheadType, bool ignoreModTypes)
{
    const char* warheadTypeString;

    if (warheadType == WARHEAD_SA)
    {
        warheadTypeString = "SA";
    }
    else if (warheadType == WARHEAD_HE)
    {
        warheadTypeString = "HE";
    }
    else if (warheadType == WARHEAD_AP)
    {
        warheadTypeString = "AP";
    }
    else if (warheadType == WARHEAD_FIRE)
    {
        warheadTypeString = "FIRE";
    }
    else if (warheadType == WARHEAD_LASER)
    {
        warheadTypeString = "LASER";
    }
    else if (warheadType == WARHEAD_PB)
    {
        warheadTypeString = "PB";
    }
    else if (warheadType == WARHEAD_FIST)
    {
        warheadTypeString = "FIST";
    }
    else if (warheadType == WARHEAD_FOOT)
    {
        warheadTypeString = "FOOT";
    }
    else if (warheadType == WARHEAD_HOLLOW_POINT)
    {
        warheadTypeString = "HOLLOW_POINT";
    }
    else if (warheadType == WARHEAD_SPORE)
    {
        warheadTypeString = "SPORE";
    }
    else if (warheadType == WARHEAD_HEADBUTT)
    {
        warheadTypeString = "HEADBUTT";
    }
    else if (warheadType == WARHEAD_FEEDME)
    {
        warheadTypeString = "FEEDME";
    }
    else if (!ignoreModTypes && warheadType < ReadWarheadCount())
    {
        warheadTypeString = GetNewWarheadIniName(warheadType);
    }
    else
    {
        ShowError("Unable to convert warhead type to string: %u", warheadType);
    }

    return warheadTypeString;
}

InfantryType ParseInfantryType(char* infantryTypeString, bool* parseError, bool ignoreModTypes)
{
    InfantryType infantryType = INFANTRY_NONE;

    if (StringsAreEqual(infantryTypeString, "NONE"))
    {
        return INFANTRY_NONE;
    }
    else if (StringsAreEqual(infantryTypeString, "RMBO"))
    {
        return INFANTRY_RAMBO;
    }
    else if (StringsAreEqual(infantryTypeString, "NIKOOMBA"))
    {
        return INFANTRY_C10;
    }
    else if (StringsAreEqual(infantryTypeString, "MOEBIUS"))
    {
        return INFANTRY_MOEBIUS;
    }
    else if (StringsAreEqual(infantryTypeString, "DELPHI"))
    {
        return INFANTRY_DELPHI;
    }
    else if (StringsAreEqual(infantryTypeString, "CHAN"))
    {
        return INFANTRY_CHAN;
    }

    if (strlen(infantryTypeString) == 2) {
        if (infantryTypeString[0] == 'E')
        {
            auto number = ParseNumber(infantryTypeString[1]);

            if (number > 0 && number < 7)
            {
                return (InfantryType)(number - 1);
            }
        }
        else if (infantryTypeString[0] == 'C')
        {
            auto number = ParseNumber(infantryTypeString[1]);

            if (number > 0 && number < 10)
            {
                return (InfantryType)(number + 6);
            }
        }
    }

    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newInfantryType = GetNewInfantryType(infantryTypeString, &matchFound);

        if (matchFound)
        {
            return (InfantryType)newInfantryType;
        }
    }

    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse infantry type from string: %s", infantryTypeString);

    return infantryType;
}

InfantryType ParseInfantryType(const char* infantryTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(infantryTypeString))
    {
        ShowError("Infantry type passed to ParseInfantryType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return INFANTRY_NONE;
    }

    auto infantryTypeStr = strdup(infantryTypeString);

    InfantryType result = ParseInfantryType(infantryTypeStr, parseError, ignoreModTypes);

    delete infantryTypeStr;

    return result;
}

const char* InfantryTypeToString(InfantryType infantryType, bool ignoreModTypes)
{
    const char* infantryTypeString;

    if (infantryType == INFANTRY_NONE)
    {
        infantryTypeString = "NONE";
    }
    else if (infantryType == INFANTRY_RAMBO)
    {
        infantryTypeString = "RMBO";
    }
    else if (infantryType == INFANTRY_C10)
    {
        infantryTypeString = "NIKOOMBA";
    }
    else if (infantryType == INFANTRY_MOEBIUS)
    {
        infantryTypeString = "MOEBIUS";
    }
    else if (infantryType == INFANTRY_DELPHI)
    {
        infantryTypeString = "DELPHI";
    }
    else if (infantryType == INFANTRY_CHAN)
    {
        infantryTypeString = "CHAN";
    }
    else if (infantryType > -1 && infantryType < 6)
    {
        auto index = infantryType;

        infantryTypeString = INFANTRY_TYPE_MAP[index];
    }
    else if (infantryType > 6 && infantryType < 17)
    {
        auto index = infantryType - 7;

        infantryTypeString = CIVILIAN_TYPE_MAP[index];
    }
    else if (!ignoreModTypes && infantryType < ReadInfantryCount())
    {
        infantryTypeString = GetNewInfantryIniName(infantryType);
    }
    else
    {
        ShowError("Unable to convert infantry type to string: %d", infantryType);
    }

    return infantryTypeString;
}

UnitType ParseUnitType(char* unitTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringsAreEqual(unitTypeString, "NONE"))
    {
        return UNIT_NONE;
    }
    else if (StringsAreEqual(unitTypeString, "HTNK"))
    {
        return UNIT_HTANK;
    }
    else if (StringsAreEqual(unitTypeString, "MTNK"))
    {
        return UNIT_MTANK;
    }
    else if (StringsAreEqual(unitTypeString, "LTNK"))
    {
        return UNIT_LTANK;
    }
    else if (StringsAreEqual(unitTypeString, "STNK"))
    {
        return UNIT_STANK;
    }
    else if (StringsAreEqual(unitTypeString, "FTNK"))
    {
        return UNIT_FTANK;
    }
    else if (StringsAreEqual(unitTypeString, "VICE"))
    {
        return UNIT_VICE;
    }
    else if (StringsAreEqual(unitTypeString, "APC"))
    {
        return UNIT_APC;
    }
    else if (StringsAreEqual(unitTypeString, "MSAM"))
    {
        return UNIT_MLRS;
    }
    else if (StringsAreEqual(unitTypeString, "JEEP"))
    {
        return UNIT_JEEP;
    }
    else if (StringsAreEqual(unitTypeString, "BGGY"))
    {
        return UNIT_BUGGY;
    }
    else if (StringsAreEqual(unitTypeString, "HARV"))
    {
        return UNIT_HARVESTER;
    }
    else if (StringsAreEqual(unitTypeString, "ARTY"))
    {
        return UNIT_ARTY;
    }
    else if (StringsAreEqual(unitTypeString, "MLRS"))
    {
        return UNIT_MSAM;
    }
    else if (StringsAreEqual(unitTypeString, "LST"))
    {
        return UNIT_HOVER;
    }
    else if (StringsAreEqual(unitTypeString, "MHQ"))
    {
        return UNIT_MHQ;
    }
    else if (StringsAreEqual(unitTypeString, "BOAT"))
    {
        return UNIT_GUNBOAT;
    }
    else if (StringsAreEqual(unitTypeString, "MCV"))
    {
        return UNIT_MCV;
    }
    else if (StringsAreEqual(unitTypeString, "BIKE"))
    {
        return UNIT_BIKE;
    }
    else if (StringsAreEqual(unitTypeString, "TRIC"))
    {
        return UNIT_TRIC;
    }
    else if (StringsAreEqual(unitTypeString, "TREX"))
    {
        return UNIT_TREX;
    }
    else if (StringsAreEqual(unitTypeString, "RAPT"))
    {
        return UNIT_RAPT;
    }
    else if (StringsAreEqual(unitTypeString, "STEG"))
    {
        return UNIT_STEG;
    }
    else if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newUnitType = GetNewUnitType(unitTypeString, &matchFound);

        if (matchFound)
        {
            return (UnitType)newUnitType;
        }
    }

    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse unit type from string: %s", unitTypeString);

    return UNIT_NONE;
}

UnitType ParseUnitType(const char* unitTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(unitTypeString))
    {
        ShowError("Unit type passed to ParseUnitType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return UNIT_NONE;
    }

    auto unitTypeStr = strdup(unitTypeString);

    UnitType result = ParseUnitType(unitTypeStr, parseError, ignoreModTypes);

    delete unitTypeStr;

    return result;
}

const char* UnitTypeToString(UnitType unitType, bool ignoreModTypes)
{
    const char* unitTypeString;

    if (unitType == UNIT_NONE)
    {
        unitTypeString = "NONE";
    }
    else if (unitType == UNIT_HTANK)
    {
        unitTypeString = "HTNK";
    }
    else if (unitType == UNIT_MTANK)
    {
        unitTypeString = "MTNK";
    }
    else if (unitType == UNIT_LTANK)
    {
        unitTypeString = "LTNK";
    }
    else if (unitType == UNIT_STANK)
    {
        unitTypeString = "STNK";
    }
    else if (unitType == UNIT_FTANK)
    {
        unitTypeString = "FTNK";
    }
    else if (unitType == UNIT_VICE)
    {
        unitTypeString = "VICE";
    }
    else if (unitType == UNIT_APC)
    {
        unitTypeString = "APC";
    }
    else if (unitType == UNIT_MLRS)
    {
        unitTypeString = "MLRS";
    }
    else if (unitType == UNIT_JEEP)
    {
        unitTypeString = "JEEP";
    }
    else if (unitType == UNIT_BUGGY)
    {
        unitTypeString = "BGGY";
    }
    else if (unitType == UNIT_HARVESTER)
    {
        unitTypeString = "HARV";
    }
    else if (unitType == UNIT_ARTY)
    {
        unitTypeString = "ARTY";
    }
    else if (unitType == UNIT_MSAM)
    {
        unitTypeString = "MSAM";
    }
    else if (unitType == UNIT_HOVER)
    {
        unitTypeString = "LST";
    }
    else if (unitType == UNIT_MHQ)
    {
        unitTypeString = "MHQ";
    }
    else if (unitType == UNIT_GUNBOAT)
    {
        unitTypeString = "BOAT";
    }
    else if (unitType == UNIT_MCV)
    {
        unitTypeString = "MCV";
    }
    else if (unitType == UNIT_BIKE)
    {
        unitTypeString = "BIKE";
    }
    else if (unitType == UNIT_TRIC)
    {
        unitTypeString = "TRIC";
    }
    else if (unitType == UNIT_TREX)
    {
        unitTypeString = "TREX";
    }
    else if (unitType == UNIT_RAPT)
    {
        unitTypeString = "RAPT";
    }
    else if (unitType == UNIT_STEG)
    {
        unitTypeString = "STEG";
    }
    else if (!ignoreModTypes && unitType < ReadUnitCount())
    {
        unitTypeString = GetNewUnitIniName(unitType);
    }
    else
    {
        ShowError("Unable to convert unit type to string: %d", unitType);
    }

    return unitTypeString;
}

SpeedType ParseUnitSpeedType(char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes)
{
    SpeedType speedType = SPEED_NONE;

    if (StringsAreEqual(unitSpeedTypeString, "NONE"))
    {
        speedType = SPEED_NONE;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "FOOT"))
    {
        speedType = SPEED_FOOT;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "TRACK"))
    {
        speedType = SPEED_TRACK;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "HARVESTER"))
    {
        speedType = SPEED_HARVESTER;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "WHEEL"))
    {
        speedType = SPEED_WHEEL;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "WINGED"))
    {
        speedType = SPEED_WINGED;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "HOVER"))
    {
        speedType = SPEED_HOVER;
    }
    else if (StringsAreEqual(unitSpeedTypeString, "FLOAT"))
    {
        speedType = SPEED_FLOAT;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        ShowError("Unable to parse unit speed type from string: %s", unitSpeedTypeString);
    }

    return speedType;
}

SpeedType ParseUnitSpeedType(const char* unitSpeedTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(unitSpeedTypeString))
    {
        ShowError("Unit speed type passed to ParseUnitSpeedType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return SPEED_NONE;
    }

    auto unitSpeedTypeStr = strdup(unitSpeedTypeString);

    auto unitSpeedType = ParseUnitSpeedType(unitSpeedTypeStr, parseError, ignoreModTypes);

    delete unitSpeedTypeStr;

    return unitSpeedType;
}

const char* UnitSpeedTypeToString(SpeedType unitSpeedType)
{
    const char* unitSpeedTypeString;

    if (unitSpeedType == SPEED_NONE)
    {
        unitSpeedTypeString = "NONE";
    }
    else if (unitSpeedType == SPEED_FOOT)
    {
        unitSpeedTypeString = "FOOT";
    }
    else if (unitSpeedType == SPEED_TRACK)
    {
        unitSpeedTypeString = "TRACK";
    }
    else if (unitSpeedType == SPEED_HARVESTER)
    {
        unitSpeedTypeString = "HARVESTER";
    }
    else if (unitSpeedType == SPEED_WHEEL)
    {
        unitSpeedTypeString = "WHEEL";
    }
    else if (unitSpeedType == SPEED_WINGED)
    {
        unitSpeedTypeString = "WINGED";
    }
    else if (unitSpeedType == SPEED_HOVER)
    {
        unitSpeedTypeString = "HOVER";
    }
    else if (unitSpeedType == SPEED_FLOAT)
    {
        unitSpeedTypeString = "FLOAT";
    }
    else
    {
        ShowError("Unable to convert unit speed type to string: %d", unitSpeedType);
    }

    return unitSpeedTypeString;
}

AircraftType ParseAircraftType(char* aircraftTypeString, bool* parseError, bool ignoreModTypes)
{
    AircraftType aircraftType = AIRCRAFT_NONE;

    if (StringsAreEqual(aircraftTypeString, "NONE"))
    {
        return AIRCRAFT_NONE;
    }
    else if (StringsAreEqual(aircraftTypeString, "A10"))
    {
        return AIRCRAFT_A10;
    }
    else if (StringsAreEqual(aircraftTypeString, "C17"))
    {
        return AIRCRAFT_CARGO;
    }
    else if (StringsAreEqual(aircraftTypeString, "HELI"))
    {
        return AIRCRAFT_HELICOPTER;
    }
    else if (StringsAreEqual(aircraftTypeString, "ORCA"))
    {
        return AIRCRAFT_ORCA;
    }
    else if (StringsAreEqual(aircraftTypeString, "TRAN"))
    {
        return AIRCRAFT_TRANSPORT;
    }
    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newAircraftType = GetNewAircraftType(aircraftTypeString, &matchFound);

        if (matchFound)
        {
            return (AircraftType)newAircraftType;
        }
    }
    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse aircraft type from string: %s", aircraftTypeString);

    return aircraftType;
}

AircraftType ParseAircraftType(const char* aircraftTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(aircraftTypeString))
    {
        ShowError("Aircraft type passed to ParseAircraftType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return AIRCRAFT_NONE;
    }

    auto aircraftTypeStr = strdup(aircraftTypeString);

    auto aircraftType = ParseAircraftType(aircraftTypeStr, parseError, ignoreModTypes);

    delete aircraftTypeStr;

    return aircraftType;
}

const char* AircraftTypeToString(AircraftType aircraftType, bool ignoreModTypes)
{
    const char* aircraftTypeString;

    if (aircraftType == AIRCRAFT_NONE)
    {
        aircraftTypeString = "NONE";
    }
    else if (aircraftType == AIRCRAFT_TRANSPORT)
    {
        aircraftTypeString = "TRAN";
    }
    else if (aircraftType == AIRCRAFT_A10)
    {
        aircraftTypeString = "A10";
    }
    else if (aircraftType == AIRCRAFT_HELICOPTER)
    {
        aircraftTypeString = "HELI";
    }
    else if (aircraftType == AIRCRAFT_CARGO)
    {
        aircraftTypeString = "C17";
    }
    else if (aircraftType == AIRCRAFT_ORCA)
    {
        aircraftTypeString = "ORCA";
    }
    else if (!ignoreModTypes && aircraftType < ReadInfantryCount())
    {
        aircraftTypeString = GetNewAircraftIniName(aircraftType);
    }
    else
    {
        ShowError("Unable to convert aircraft type to string: %d", aircraftType);
    }

    return aircraftTypeString;
}

StructType ParseStructureType(char* structTypeString, bool* parseError, bool ignoreModTypes)
{
    StructType structType = STRUCT_NONE;

    if (StringsAreEqual(structTypeString, "NONE"))
    {
        return STRUCT_NONE;
    }
    else if (StringsAreEqual(structTypeString, "WEAP"))
    {
        return STRUCT_WEAP;
    }
    else if (StringsAreEqual(structTypeString, "GTWR"))
    {
        return STRUCT_GTOWER;
    }
    else if (StringsAreEqual(structTypeString, "ATWR"))
    {
        return STRUCT_ATOWER;
    }
    else if (StringsAreEqual(structTypeString, "OBLI"))
    {
        return STRUCT_OBELISK;
    }
    else if (StringsAreEqual(structTypeString, "HQ"))
    {
        return STRUCT_RADAR;
    }
    else if (StringsAreEqual(structTypeString, "GUN"))
    {
        return STRUCT_TURRET;
    }
    else if (StringsAreEqual(structTypeString, "FACT"))
    {
        return STRUCT_CONST;
    }
    else if (StringsAreEqual(structTypeString, "PROC"))
    {
        return STRUCT_REFINERY;
    }
    else if (StringsAreEqual(structTypeString, "SILO"))
    {
        return STRUCT_STORAGE;
    }
    else if (StringsAreEqual(structTypeString, "HPAD"))
    {
        return STRUCT_HELIPAD;
    }
    else if (StringsAreEqual(structTypeString, "SAM"))
    {
        return STRUCT_SAM;
    }
    else if (StringsAreEqual(structTypeString, "AFLD"))
    {
        return STRUCT_AIRSTRIP;
    }
    else if (StringsAreEqual(structTypeString, "NUKE"))
    {
        return STRUCT_POWER;
    }
    else if (StringsAreEqual(structTypeString, "NUK2"))
    {
        return STRUCT_ADVANCED_POWER;
    }
    else if (StringsAreEqual(structTypeString, "HOSP"))
    {
        return STRUCT_HOSPITAL;
    }
    else if (StringsAreEqual(structTypeString, "PYLE"))
    {
        return STRUCT_BARRACKS;
    }
    else if (StringsAreEqual(structTypeString, "ARCO"))
    {
        return STRUCT_TANKER;
    }
    else if (StringsAreEqual(structTypeString, "FIX"))
    {
        return STRUCT_REPAIR;
    }
    else if (StringsAreEqual(structTypeString, "BIO"))
    {
        return STRUCT_BIO_LAB;
    }
    else if (StringsAreEqual(structTypeString, "HAND"))
    {
        return STRUCT_HAND;
    }
    else if (StringsAreEqual(structTypeString, "TMPL"))
    {
        return STRUCT_TEMPLE;
    }
    else if (StringsAreEqual(structTypeString, "EYE"))
    {
        return STRUCT_EYE;
    }
    else if (StringsAreEqual(structTypeString, "MISS"))
    {
        return STRUCT_MISSION;
    }
    else if (StringsAreEqual(structTypeString, "V01"))
    {
        return STRUCT_V01;
    }
    else if (StringsAreEqual(structTypeString, "V02"))
    {
        return STRUCT_V02;
    }
    else if (StringsAreEqual(structTypeString, "V03"))
    {
        return STRUCT_V03;
    }
    else if (StringsAreEqual(structTypeString, "V04"))
    {
        return STRUCT_V04;
    }
    else if (StringsAreEqual(structTypeString, "V05"))
    {
        return STRUCT_V05;
    }
    else if (StringsAreEqual(structTypeString, "V06"))
    {
        return STRUCT_V06;
    }
    else if (StringsAreEqual(structTypeString, "V07"))
    {
        return STRUCT_V07;
    }
    else if (StringsAreEqual(structTypeString, "V08"))
    {
        return STRUCT_V08;
    }
    else if (StringsAreEqual(structTypeString, "V09"))
    {
        return STRUCT_V09;
    }
    else if (StringsAreEqual(structTypeString, "V10"))
    {
        return STRUCT_V10;
    }
    else if (StringsAreEqual(structTypeString, "V11"))
    {
        return STRUCT_V11;
    }
    else if (StringsAreEqual(structTypeString, "V12"))
    {
        return STRUCT_V12;
    }
    else if (StringsAreEqual(structTypeString, "V13"))
    {
        return STRUCT_V13;
    }
    else if (StringsAreEqual(structTypeString, "V14"))
    {
        return STRUCT_V14;
    }
    else if (StringsAreEqual(structTypeString, "V15"))
    {
        return STRUCT_V15;
    }
    else if (StringsAreEqual(structTypeString, "V16"))
    {
        return STRUCT_V16;
    }
    else if (StringsAreEqual(structTypeString, "V17"))
    {
        return STRUCT_V17;
    }
    else if (StringsAreEqual(structTypeString, "V18"))
    {
        return STRUCT_V18;
    }
    else if (StringsAreEqual(structTypeString, "V19"))
    {
        return STRUCT_PUMP;
    }
    else if (StringsAreEqual(structTypeString, "V20"))
    {
        return STRUCT_V20;
    }
    else if (StringsAreEqual(structTypeString, "V21"))
    {
        return STRUCT_V21;
    }
    else if (StringsAreEqual(structTypeString, "V22"))
    {
        return STRUCT_V22;
    }
    else if (StringsAreEqual(structTypeString, "V23"))
    {
        return STRUCT_V23;
    }
    else if (StringsAreEqual(structTypeString, "V24"))
    {
        return STRUCT_V24;
    }
    else if (StringsAreEqual(structTypeString, "V25"))
    {
        return STRUCT_V25;
    }
    else if (StringsAreEqual(structTypeString, "V26"))
    {
        return STRUCT_V26;
    }
    else if (StringsAreEqual(structTypeString, "V27"))
    {
        return STRUCT_V27;
    }
    else if (StringsAreEqual(structTypeString, "V28"))
    {
        return STRUCT_V28;
    }
    else if (StringsAreEqual(structTypeString, "V29"))
    {
        return STRUCT_V29;
    }
    else if (StringsAreEqual(structTypeString, "V30"))
    {
        return STRUCT_V30;
    }
    else if (StringsAreEqual(structTypeString, "V31"))
    {
        return STRUCT_V31;
    }
    else if (StringsAreEqual(structTypeString, "V32"))
    {
        return STRUCT_V32;
    }
    else if (StringsAreEqual(structTypeString, "V33"))
    {
        return STRUCT_V33;
    }
    else if (StringsAreEqual(structTypeString, "V34"))
    {
        return STRUCT_V34;
    }
    else if (StringsAreEqual(structTypeString, "V35"))
    {
        return STRUCT_V35;
    }
    else if (StringsAreEqual(structTypeString, "V36"))
    {
        return STRUCT_V36;
    }
    else if (StringsAreEqual(structTypeString, "V37"))
    {
        return STRUCT_V37;
    }
    else if (StringsAreEqual(structTypeString, "SBAG"))
    {
        return STRUCT_SANDBAG_WALL;
    }
    else if (StringsAreEqual(structTypeString, "CYCL"))
    {
        return STRUCT_CYCLONE_WALL;
    }
    else if (StringsAreEqual(structTypeString, "BRIK"))
    {
        return STRUCT_BRICK_WALL;
    }
    else if (StringsAreEqual(structTypeString, "BARB"))
    {
        return STRUCT_BARBWIRE_WALL;
    }
    else if (StringsAreEqual(structTypeString, "WOOD"))
    {
        return STRUCT_WOOD_WALL;
    }
    if (!ignoreModTypes)
    {
        bool matchFound = false;
        auto newStructType = GetNewBuildingType(structTypeString, &matchFound);

        if (matchFound)
        {
            return (StructType)newStructType;
        }
    }
    if (parseError != NULL)
    {
        *parseError = true;
    }

    ShowError("Unable to parse structure from string: %s", structTypeString);

    return structType;
}

StructType ParseStructureType(const char* structTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(structTypeString))
    {
        ShowError("Structure type passed to ParseStructureType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return STRUCT_NONE;
    }

    auto structTypeStr = strdup(structTypeString);

    auto structType = ParseStructureType(structTypeStr, parseError, ignoreModTypes);

    delete structTypeStr;

    return structType;
}

const char* StructureTypeToString(StructType structType, bool ignoreModTypes)
{
    const char* structTypeString;

    if (structType == STRUCT_NONE)
    {
        structTypeString = "NONE";
    }
    else if (structType == STRUCT_WEAP)
    {
        structTypeString = "WEAP";
    }
    else if (structType == STRUCT_GTOWER)
    {
        structTypeString = "GTWR";
    }
    else if (structType == STRUCT_ATOWER)
    {
        structTypeString = "ATWR";
    }
    else if (structType == STRUCT_OBELISK)
    {
        structTypeString = "OBLI";
    }
    else if (structType == STRUCT_RADAR)
    {
        structTypeString = "HQ";
    }
    else if (structType == STRUCT_TURRET)
    {
        structTypeString = "GUN";
    }
    else if (structType == STRUCT_CONST)
    {
        structTypeString = "FACT";
    }
    else if (structType == STRUCT_REFINERY)
    {
        structTypeString = "PROC";
    }
    else if (structType == STRUCT_STORAGE)
    {
        structTypeString = "SILO";
    }
    else if (structType == STRUCT_HELIPAD)
    {
        structTypeString = "HPAD";
    }
    else if (structType == STRUCT_SAM)
    {
        structTypeString = "SAM";
    }
    else if (structType == STRUCT_AIRSTRIP)
    {
        structTypeString = "AFLD";
    }
    else if (structType == STRUCT_POWER)
    {
        structTypeString = "NUKE";
    }
    else if (structType == STRUCT_ADVANCED_POWER)
    {
        structTypeString = "NUK2";
    }
    else if (structType == STRUCT_HOSPITAL)
    {
        structTypeString = "HOSP";
    }
    else if (structType == STRUCT_BARRACKS)
    {
        structTypeString = "PYLE";
    }
    else if (structType == STRUCT_TANKER)
    {
        structTypeString = "ARCO";
    }
    else if (structType == STRUCT_REPAIR)
    {
        structTypeString = "FIX";
    }
    else if (structType == STRUCT_BIO_LAB)
    {
        structTypeString = "BIO";
    }
    else if (structType == STRUCT_HAND)
    {
        structTypeString = "HAND";
    }
    else if (structType == STRUCT_TEMPLE)
    {
        structTypeString = "TMPL";
    }
    else if (structType == STRUCT_EYE)
    {
        structTypeString = "EYE";
    }
    else if (structType == STRUCT_MISSION)
    {
        structTypeString = "MISS";
    }
    else if (structType == STRUCT_V01)
    {
        structTypeString = "V01";
    }
    else if (structType == STRUCT_V02)
    {
        structTypeString = "V02";
    }
    else if (structType == STRUCT_V03)
    {
        structTypeString = "V03";
    }
    else if (structType == STRUCT_V04)
    {
        structTypeString = "V04";
    }
    else if (structType == STRUCT_V05)
    {
        structTypeString = "V05";
    }
    else if (structType == STRUCT_V06)
    {
        structTypeString = "V06";
    }
    else if (structType == STRUCT_V07)
    {
        structTypeString = "V07";
    }
    else if (structType == STRUCT_V08)
    {
        structTypeString = "V08";
    }
    else if (structType == STRUCT_V09)
    {
        structTypeString = "V09";
    }
    else if (structType == STRUCT_V10)
    {
        structTypeString = "V10";
    }
    else if (structType == STRUCT_V11)
    {
        structTypeString = "V11";
    }
    else if (structType == STRUCT_V12)
    {
        structTypeString = "V12";
    }
    else if (structType == STRUCT_V13)
    {
        structTypeString = "V13";
    }
    else if (structType == STRUCT_V14)
    {
        structTypeString = "V14";
    }
    else if (structType == STRUCT_V15)
    {
        structTypeString = "V15";
    }
    else if (structType == STRUCT_V16)
    {
        structTypeString = "V16";
    }
    else if (structType == STRUCT_V17)
    {
        structTypeString = "V17";
    }
    else if (structType == STRUCT_V18)
    {
        structTypeString = "V18";
    }
    else if (structType == STRUCT_PUMP)
    {
        structTypeString = "V19";
    }
    else if (structType == STRUCT_V20)
    {
        structTypeString = "V20";
    }
    else if (structType == STRUCT_V21)
    {
        structTypeString = "V21";
    }
    else if (structType == STRUCT_V22)
    {
        structTypeString = "V22";
    }
    else if (structType == STRUCT_V23)
    {
        structTypeString = "V23";
    }
    else if (structType == STRUCT_V24)
    {
        structTypeString = "V24";
    }
    else if (structType == STRUCT_V25)
    {
        structTypeString = "V25";
    }
    else if (structType == STRUCT_V26)
    {
        structTypeString = "V26";
    }
    else if (structType == STRUCT_V27)
    {
        structTypeString = "V27";
    }
    else if (structType == STRUCT_V28)
    {
        structTypeString = "V28";
    }
    else if (structType == STRUCT_V29)
    {
        structTypeString = "V29";
    }
    else if (structType == STRUCT_V30)
    {
        structTypeString = "V30";
    }
    else if (structType == STRUCT_V31)
    {
        structTypeString = "V31";
    }
    else if (structType == STRUCT_V32)
    {
        structTypeString = "V32";
    }
    else if (structType == STRUCT_V33)
    {
        structTypeString = "V33";
    }
    else if (structType == STRUCT_V34)
    {
        structTypeString = "V34";
    }
    else if (structType == STRUCT_V35)
    {
        structTypeString = "V35";
    }
    else if (structType == STRUCT_V36)
    {
        structTypeString = "V36";
    }
    else if (structType == STRUCT_V37)
    {
        structTypeString = "V37";
    }
    else if (structType == STRUCT_SANDBAG_WALL)
    {
        structTypeString = "SBAG";
    }
    else if (structType == STRUCT_CYCLONE_WALL)
    {
        structTypeString = "CYCL";
    }
    else if (structType == STRUCT_BRICK_WALL)
    {
        structTypeString = "BRIK";
    }
    else if (structType == STRUCT_BARBWIRE_WALL)
    {
        structTypeString = "BARB";
    }
    else if (structType == STRUCT_WOOD_WALL)
    {
        structTypeString = "WOOD";
    }
    else if (!ignoreModTypes && structType < ReadBuildingCount())
    {
        structTypeString = GetNewBuildingIniName(structType);
    }
    else
    {
        ShowError("Unable to convert structure type to string: %d", structType);
    }

    return structTypeString;
}

long StructureTypeToPrerequisite(StructType structType, bool* parseError, bool ignoreModTypes)
{
    if (structType == STRUCT_NONE)
    {
        return STRUCTF_NONE;
    }

    auto structCount = ignoreModTypes ? STRUCT_COUNT : ReadBuildingCount();

    if (structType < STRUCT_FIRST || structType > structCount - 1)
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        return STRUCTF_NONE;
    }

    return 1L << structType;
}

FactoryType ParseFactoryType(char* factoryTypeString, bool* parseError, bool ignoreModTypes)
{
    FactoryType factoryType;

    if (StringsAreEqual(factoryTypeString, "NONE"))
    {
        factoryType = FACTORY_TYPE_NONE;
    }
    else if (StringsAreEqual(factoryTypeString, "INFANTRY"))
    {
        factoryType = FACTORY_TYPE_INFANTRY;
    }
    else if (StringsAreEqual(factoryTypeString, "UNIT"))
    {
        factoryType = FACTORY_TYPE_UNIT;
    }
    else if (StringsAreEqual(factoryTypeString, "AIRCRAFT"))
    {
        factoryType = FACTORY_TYPE_AIRCRAFT;
    }
    else if (StringsAreEqual(factoryTypeString, "BUILDING"))
    {
        factoryType = FACTORY_TYPE_BUILDING;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        ShowError("Unable to parse factory type from string: %s", factoryTypeString);
    }

    return factoryType;
}

FactoryType ParseFactoryType(const char* factoryTypeString, bool* parseError, bool ignoreModTypes)
{
    if (StringIsEmpty(factoryTypeString))
    {
        ShowError("Factory type passed to ParseFactoryType was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return FACTORY_TYPE_NONE;
    }

    auto factoryTypeStr = strdup(factoryTypeString);

    auto factoryType = ParseFactoryType(factoryTypeStr, parseError, ignoreModTypes);

    delete factoryTypeStr;

    return factoryType;
}

const char* FactoryTypeToString(FactoryType factoryType)
{
    const char* factoryTypeString;

    if (factoryType == FACTORY_TYPE_NONE)
    {
        factoryTypeString = "NONE";
    }
    else if (factoryType == FACTORY_TYPE_INFANTRY)
    {
        factoryTypeString = "INFANTRY";
    }
    else if (factoryType == FACTORY_TYPE_UNIT)
    {
        factoryTypeString = "UNIT";
    }
    else if (factoryType == FACTORY_TYPE_AIRCRAFT)
    {
        factoryTypeString = "AIRCRAFT";
    }
    else if (factoryType == FACTORY_TYPE_BUILDING)
    {
        factoryTypeString = "BUILDING";
    }
    else
    {
        ShowError("Unable to convert factory type to string: %d", factoryType);
    }

    return factoryTypeString;
}
