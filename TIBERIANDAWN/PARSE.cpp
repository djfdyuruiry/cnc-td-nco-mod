#include "rules_ini.h"
#include "RulesIniInfo.h"
#include "strings.h"
#include "utils.h"

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

DiffType Parse_Difficulty_Type(char* difficultyTypeString, bool* parseError)
{
    DiffType difficultyType;

    if (Strings_Are_Equal(difficultyTypeString, "EASY"))
    {
        difficultyType = DIFF_EASY;
    }
    else if (Strings_Are_Equal(difficultyTypeString, "NORMAL"))
    {
        difficultyType = DIFF_NORMAL;
    }
    else if (Strings_Are_Equal(difficultyTypeString, "HARD"))
    {
        difficultyType = DIFF_HARD;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse difficulty from string: %s", difficultyTypeString);
    }

    return difficultyType;
}

const char* Difficulty_Type_To_String(DiffType difficultyType)
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
        Show_Error("Unable to convert difficulty type to string: %d", difficultyType);
    }

    return difficultyTypeString;
}

HousesType Parse_House_Type(char* houseTypeString, bool* parseError)
{
    HousesType house;

    if (Strings_Are_Equal(houseTypeString, "NONE"))
    {
        house = HOUSE_NONE;
    }
    else if (Strings_Are_Equal(houseTypeString, "GOODGUY"))
    {
        house = HOUSE_GOOD;
    }
    else if (Strings_Are_Equal(houseTypeString, "BADGUY"))
	{
		house = HOUSE_BAD;
	}
	else if (Strings_Are_Equal(houseTypeString, "NEUTRAL"))
	{
		house = HOUSE_NEUTRAL;
	}
	else if (Strings_Are_Equal(houseTypeString, "JP"))
	{
		house = HOUSE_JP;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI1"))
	{
		house = HOUSE_MULTI1;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI2"))
	{
        house = HOUSE_MULTI2;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI3"))
	{
        house = HOUSE_MULTI3;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI4"))
	{
        house = HOUSE_MULTI4;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI5"))
	{
        house = HOUSE_MULTI5;
	}
	else if (Strings_Are_Equal(houseTypeString, "MULTI6"))
	{
        house = HOUSE_MULTI6;
	}
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse house from string: %s", houseTypeString);
    }

	return house;
}

HousesType Parse_House_Type(const char* houseTypeString, bool* parseError)
{
    auto houseTypeStr = strdup(houseTypeString);

    auto houseType = Parse_House_Type(houseTypeStr, parseError);

    delete houseTypeStr;

    return houseType;
}

int Parse_House_Name_List_Csv(char* houseListCsv, bool* parseError)
{
    auto houseNameListSize = 0u;
    auto houseNameList = Parse_Csv_String(houseListCsv, HOUSE_NAME_MAX_LENGTH, &houseNameListSize);

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
        auto house = Parse_House_Type(houseNameList[i], &houseParseError);

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

int Parse_House_Name_List_Csv(const char* houseListCsv, bool* parseError)
{
    if (String_Is_Empty(houseListCsv))
    {
        Show_Error("CSV passed to Parse_House_Name_List_Csv was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return HOUSEF_NONE;
    }

    auto houseTypeCsvStr = strdup(houseListCsv);

    auto owner = Parse_House_Name_List_Csv(houseTypeCsvStr, parseError);

    delete houseTypeCsvStr;

    return owner;
}

const char* House_Type_To_String(HousesType houseType)
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
        Show_Error("Unable to convert house type to string: %d", houseType);
    }

    return houseTypeString;
}

StructType Prerequisite_To_Structure_Type(long prerequisite)
{
    auto structType = STRUCT_NONE;
    
    if (prerequisite == STRUCTF_NONE || prerequisite < 0l)
    {
        structType = STRUCT_NONE;
    }
    else if (prerequisite == STRUCTF_ADVANCED_POWER)
    {
        structType = STRUCT_ADVANCED_POWER;
    }
    else if (prerequisite == STRUCTF_REPAIR)
    {
        structType = STRUCT_REPAIR;
    }
    else if (prerequisite == STRUCTF_EYE)
    {
        structType = STRUCT_EYE;
    }
    else if (prerequisite == STRUCTF_TEMPLE)
    {
        structType = STRUCT_TEMPLE;
    }
    else if (prerequisite == STRUCTF_HAND)
    {
        structType = STRUCT_HAND;
    }
    else if (prerequisite == STRUCTF_BIO_LAB)
    {
        structType = STRUCT_BIO_LAB;
    }
    else if (prerequisite == STRUCTF_OBELISK)
    {
        structType = STRUCT_OBELISK;
    }
    else if (prerequisite == STRUCTF_ATOWER)
    {
        structType = STRUCT_ATOWER;
    }
    else if (prerequisite == STRUCTF_WEAP)
    {
        structType = STRUCT_WEAP;
    }
    else if (prerequisite == STRUCTF_GTOWER)
    {
        structType = STRUCT_GTOWER;
    }
    else if (prerequisite == STRUCTF_RADAR)
    {
        structType = STRUCT_RADAR;
    }
    else if (prerequisite == STRUCTF_TURRET)
    {
        structType = STRUCT_TURRET;
    }
    else if (prerequisite == STRUCTF_CONST)
    {
        structType = STRUCT_CONST;
    }
    else if (prerequisite == STRUCTF_REFINERY)
    {
        structType = STRUCT_REFINERY;
    }
    else if (prerequisite == STRUCTF_STORAGE)
    {
        structType = STRUCT_STORAGE;
    }
    else if (prerequisite == STRUCTF_HELIPAD)
    {
        structType = STRUCT_HELIPAD;
    }
    else if (prerequisite == STRUCTF_SAM)
    {
        structType = STRUCT_SAM;
    }
    else if (prerequisite == STRUCTF_AIRSTRIP)
    {
        structType = STRUCT_AIRSTRIP;
    }
    else if (prerequisite == STRUCTF_POWER)
    {
        structType = STRUCT_POWER;
    }
    else if (prerequisite == STRUCTF_HOSPITAL)
    {
        structType = STRUCT_HOSPITAL;
    }
    else if (prerequisite == STRUCTF_BARRACKS)
    {
        structType = STRUCT_BARRACKS;
    }
    else if (prerequisite == STRUCTF_TANKER)
    {
        structType = STRUCT_TANKER;
    }
    else if (prerequisite == STRUCTF_MISSION)
    {
        structType = STRUCT_MISSION;
    }
    else
    {
        Show_Error("Unable to convert prerequiste to structure type: %d", structType);
    }

    return structType;
}

const char* Prerequisite_To_String(long prerequisite)
{
    return Structure_Type_To_String(
        Prerequisite_To_Structure_Type(prerequisite)
    );
}

WeaponType Parse_Weapon_Type(char* weaponTypeString, bool* parseError, bool ignoreModTypes)
{
    WeaponType weaponType;

    if (Strings_Are_Equal(weaponTypeString, "NONE"))
    {
        weaponType = WEAPON_NONE;
    }
    else if (Strings_Are_Equal(weaponTypeString, "RIFLE"))
    {
        weaponType = WEAPON_RIFLE;
    }
    else if (Strings_Are_Equal(weaponTypeString, "CHAIN_GUN"))
    {
        weaponType = WEAPON_CHAIN_GUN;
    }
    else if (Strings_Are_Equal(weaponTypeString, "PISTOL"))
    {
        weaponType = WEAPON_PISTOL;
    }
    else if (Strings_Are_Equal(weaponTypeString, "M16"))
    {
        weaponType = WEAPON_M16;
    }
    else if (Strings_Are_Equal(weaponTypeString, "DRAGON"))
    {
        weaponType = WEAPON_DRAGON;
    }
    else if (Strings_Are_Equal(weaponTypeString, "FLAMETHROWER"))
    {
        weaponType = WEAPON_FLAMETHROWER;
    }
    else if (Strings_Are_Equal(weaponTypeString, "FLAME_TONGUE"))
    {
        weaponType = WEAPON_FLAME_TONGUE;
    }
    else if (Strings_Are_Equal(weaponTypeString, "CHEMSPRAY"))
    {
        weaponType = WEAPON_CHEMSPRAY;
    }
    else if (Strings_Are_Equal(weaponTypeString, "GRENADE"))
    {
        weaponType = WEAPON_GRENADE;
    }
    else if (Strings_Are_Equal(weaponTypeString, "75MM"))
    {
        weaponType = WEAPON_75MM;
    }
    else if (Strings_Are_Equal(weaponTypeString, "105MM"))
    {
        weaponType = WEAPON_105MM;
    }
    else if (Strings_Are_Equal(weaponTypeString, "120MM"))
    {
        weaponType = WEAPON_120MM;
    }
    else if (Strings_Are_Equal(weaponTypeString, "TURRET_GUN"))
    {
        weaponType = WEAPON_TURRET_GUN;
    }
    else if (Strings_Are_Equal(weaponTypeString, "MAMMOTH_TUSK"))
    {
        weaponType = WEAPON_MAMMOTH_TUSK;
    }
    else if (Strings_Are_Equal(weaponTypeString, "ROCKET_LAUNCHER"))
    {
        weaponType = WEAPON_MLRS;
    }
    else if (Strings_Are_Equal(weaponTypeString, "155MM"))
    {
        weaponType = WEAPON_155MM;
    }
    else if (Strings_Are_Equal(weaponTypeString, "M60MG"))
    {
        weaponType = WEAPON_M60MG;
    }
    else if (Strings_Are_Equal(weaponTypeString, "TOMAHAWK"))
    {
        weaponType = WEAPON_TOMAHAWK;
    }
    else if (Strings_Are_Equal(weaponTypeString, "TOWTWO"))
    {
        weaponType = WEAPON_TOW_TWO;
    }
    else if (Strings_Are_Equal(weaponTypeString, "NAPALM"))
    {
        weaponType = WEAPON_NAPALM;
    }
    else if (Strings_Are_Equal(weaponTypeString, "OBELISK_LASER"))
    {
        weaponType = WEAPON_OBELISK_LASER;
    }
    else if (Strings_Are_Equal(weaponTypeString, "SAM"))
    {
        weaponType = WEAPON_NIKE;
    }
    else if (Strings_Are_Equal(weaponTypeString, "HONEST_JOHN"))
    {
        weaponType = WEAPON_HONEST_JOHN;
    }
    else if (Strings_Are_Equal(weaponTypeString, "DINO_RAM"))
    {
        weaponType = WEAPON_STEG;
    }
    else if (Strings_Are_Equal(weaponTypeString, "DINO_BITE"))
    {
        weaponType = WEAPON_TREX;
    }
    else
    {       
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse weapon type from string: %s", weaponTypeString);
    }

    return weaponType;
}

WeaponType Parse_Weapon_Type(const char* weaponTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(weaponTypeString))
    {
        Show_Error("Weapon type passed to Parse_Weapon_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return WEAPON_NONE;
    }

    auto weaponTypeStr = strdup(weaponTypeString);

    auto weaponType = Parse_Weapon_Type(weaponTypeStr, parseError);

    delete weaponTypeStr;

    return weaponType;
}

const char* Weapon_Type_To_String(WeaponType weaponType, bool ignoreModTypes)
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
        weaponTypeString = "SAM";
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
    else
    {
        Show_Error("Unable to convert weapon type to string: %d", weaponType);
    }

    return weaponTypeString;
}

ArmorType Parse_Armor_Type(char* armorTypeString, bool* parseError)
{
    ArmorType armor;

    if (Strings_Are_Equal(armorTypeString, "NONE"))
    {
        armor = ARMOR_NONE;
    }
    else if (Strings_Are_Equal(armorTypeString, "WOOD"))
    {
        armor = ARMOR_WOOD;
    }
    else if (Strings_Are_Equal(armorTypeString, "ALUMINUM"))
    {
        armor = ARMOR_ALUMINUM;
    }
    else if (Strings_Are_Equal(armorTypeString, "STEEL"))
    {
        armor = ARMOR_STEEL;
    }
    else if (Strings_Are_Equal(armorTypeString, "CONCRETE"))
    {
        armor = ARMOR_CONCRETE;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse armor type from string: %s", armorTypeString);
    }

    return armor;
}

ArmorType Parse_Armor_Type(const char* armorTypeString, bool* parseError)
{
    if (String_Is_Empty(armorTypeString))
    {
        Show_Error("Armor type passed to Parse_Armor_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return ARMOR_NONE;
    }

    auto armorTypeStr = strdup(armorTypeString);

    auto armorType = Parse_Armor_Type(armorTypeStr, parseError);

    delete armorTypeStr;

    return armorType;
}

const char* Armor_Type_To_String(ArmorType armorType)
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
        Show_Error("Unable to convert armor type to string: %u", armorType);
    }

    return armorTypeString;
}

BulletType Parse_Bullet_Type(char* bulletTypeString, bool* parseError, bool ignoreModTypes)
{
    BulletType bullet;

    if (Strings_Are_Equal(bulletTypeString, "NONE"))
    {
        bullet = BULLET_NONE;
    }
    else if (Strings_Are_Equal(bulletTypeString, "SNIPER"))
    {
        bullet = BULLET_SNIPER;
    }
    else if (Strings_Are_Equal(bulletTypeString, "BULLET"))
    {
        bullet = BULLET_BULLET;
    }
    else if (Strings_Are_Equal(bulletTypeString, "SPREADFIRE"))
    {
        bullet = BULLET_SPREADFIRE;
    }
    else if (Strings_Are_Equal(bulletTypeString, "APDS"))
    {
        bullet = BULLET_APDS;
    }
    else if (Strings_Are_Equal(bulletTypeString, "ARTILLERY_SHELL"))
    {
        bullet = BULLET_HE;
    }
    else if (Strings_Are_Equal(bulletTypeString, "S.S.M"))
    {
        bullet = BULLET_SSM;
    }
    else if (Strings_Are_Equal(bulletTypeString, "MLRS_ROCKET"))
    {
        bullet = BULLET_SSM2;
    }
    else if (Strings_Are_Equal(bulletTypeString, "SAM_MISSILE"))
    {
        bullet = BULLET_SAM;
    }
    else if (Strings_Are_Equal(bulletTypeString, "TOW_MISSILE"))
    {
        bullet = BULLET_TOW;
    }
    else if (Strings_Are_Equal(bulletTypeString, "FLAME"))
    {
        bullet = BULLET_FLAME;
    }
    else if (Strings_Are_Equal(bulletTypeString, "CHEMICALS"))
    {
        bullet = BULLET_CHEMSPRAY;
    }
    else if (Strings_Are_Equal(bulletTypeString, "NAPALM_BOMB"))
    {
        bullet = BULLET_NAPALM;
    }
    else if (Strings_Are_Equal(bulletTypeString, "GRENADE_BOMB"))
    {
        bullet = BULLET_GRENADE;
    }
    else if (Strings_Are_Equal(bulletTypeString, "LASER_BEAM"))
    {
        bullet = BULLET_LASER;
    }
    else if (Strings_Are_Equal(bulletTypeString, "NUKE_UP"))
    {
        bullet = BULLET_NUKE_UP;
    }
    else if (Strings_Are_Equal(bulletTypeString, "NUKE_DOWN"))
    {
        bullet = BULLET_NUKE_DOWN;
    }
    else if (Strings_Are_Equal(bulletTypeString, "HONEST_JOHN_MISSILE"))
    {
        bullet = BULLET_HONEST_JOHN;
    }
    else if (Strings_Are_Equal(bulletTypeString, "GORE"))
    {
        bullet = BULLET_HEADBUTT;
    }
    else if (Strings_Are_Equal(bulletTypeString, "CHEW"))
    {
        bullet = BULLET_TREXBITE;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse bullet type from string: %s", bulletTypeString);
    }

    return bullet;
}

BulletType Parse_Bullet_Type(const char* bulletTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(bulletTypeString))
    {
        Show_Error("Bullet type passed to Parse_Bullet_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return BULLET_NONE;
    }

    auto bulletTypeStr = strdup(bulletTypeString);

    auto bulletType = Parse_Bullet_Type(bulletTypeStr, parseError);

    delete bulletTypeStr;

    return bulletType;
}

const char* Bullet_Type_To_String(BulletType bulletType, bool ignoreModTypes)
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
        bulletTypeString = "S.S.M";
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
    else
    {
        Show_Error("Unable to convert bullet type to string: %d", bulletType);
    }

    return bulletTypeString;
}

WarheadType Parse_Warhead_Type(char* warheadTypeString, bool* parseError, bool ignoreModTypes)
{
    WarheadType warhead;

    if (Strings_Are_Equal(warheadTypeString, "SA"))
    {
        warhead = WARHEAD_SA;
    }
    else if (Strings_Are_Equal(warheadTypeString, "HE"))
    {
        warhead = WARHEAD_HE;
    }
    else if (Strings_Are_Equal(warheadTypeString, "AP"))
    {
        warhead = WARHEAD_AP;
    }
    else if (Strings_Are_Equal(warheadTypeString, "FIRE"))
    {
        warhead = WARHEAD_FIRE;
    }
    else if (Strings_Are_Equal(warheadTypeString, "LASER"))
    {
        warhead = WARHEAD_LASER;
    }
    else if (Strings_Are_Equal(warheadTypeString, "PB"))
    {
        warhead = WARHEAD_PB;
    }
    else if (Strings_Are_Equal(warheadTypeString, "FIST"))
    {
        warhead = WARHEAD_FIST;
    }
    else if (Strings_Are_Equal(warheadTypeString, "FOOT"))
    {
        warhead = WARHEAD_FOOT;
    }
    else if (Strings_Are_Equal(warheadTypeString, "HOLLOW_POINT"))
    {
        warhead = WARHEAD_HOLLOW_POINT;
    }
    else if (Strings_Are_Equal(warheadTypeString, "SPORE"))
    {
        warhead = WARHEAD_SPORE;
    }
    else if (Strings_Are_Equal(warheadTypeString, "HEADBUTT"))
    {
        warhead = WARHEAD_HEADBUTT;
    }
    else if (Strings_Are_Equal(warheadTypeString, "FEEDME"))
    {
        warhead = WARHEAD_FEEDME;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse warhead type from string: %s", warheadTypeString);
    }

    return warhead;
}

WarheadType Parse_Warhead_Type(const char* warheadTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(warheadTypeString))
    {
        Show_Error("Warhead type passed to Parse_Warhead_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return WARHEAD_NONE;
    }

    auto warheadTypeStr = strdup(warheadTypeString);

    auto warheadType = Parse_Warhead_Type(warheadTypeStr, parseError);

    delete warheadTypeStr;

    return warheadType;
}

const char* Warhead_Type_To_String(WarheadType warheadType, bool ignoreModTypes)
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
    else
    {
        Show_Error("Unable to convert warhead type to string: %u", warheadType);
    }

    return warheadTypeString;
}

InfantryType Parse_Infantry_Type(char* infantryTypeString, bool* parseError, bool ignoreModTypes)
{
    InfantryType infantryType = INFANTRY_NONE;

    if (Strings_Are_Equal(infantryTypeString, "NONE"))
    {
        return INFANTRY_NONE;
    }
    else if (Strings_Are_Equal(infantryTypeString, "RMBO"))
    {
        return INFANTRY_RAMBO;
    }
    else if (Strings_Are_Equal(infantryTypeString, "NIKOOMBA"))
    {
        return INFANTRY_C10;
    }
    else if (Strings_Are_Equal(infantryTypeString, "MOEBIUS"))
    {
        return INFANTRY_MOEBIUS;
    }
    else if (Strings_Are_Equal(infantryTypeString, "DELPHI"))
    {
        return INFANTRY_DELPHI;
    }
    else if (Strings_Are_Equal(infantryTypeString, "CHAN"))
    {
        return INFANTRY_CHAN;
    }

    if (strlen(infantryTypeString) == 2) {
        if (infantryTypeString[0] == 'E')
        {
            auto number = Parse_Number(infantryTypeString[1]);

            if (number > 0 && number < 7)
            {
                return (InfantryType)(number - 1);
            }
        }
        else if (infantryTypeString[0] == 'C')
        {
            auto number = Parse_Number(infantryTypeString[1]);

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

    Show_Error("Unable to parse infantry type from string: %s", infantryTypeString);

    return infantryType;
}

InfantryType Parse_Infantry_Type(const char* infantryTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(infantryTypeString))
    {
        Show_Error("Infantry type passed to Parse_Infantry_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return INFANTRY_NONE;
    }

    auto infantryTypeStr = strdup(infantryTypeString);

    InfantryType result = Parse_Infantry_Type(infantryTypeStr, parseError, ignoreModTypes);

    delete infantryTypeStr;

    return result;
}

const char* Infantry_Type_To_String(InfantryType infantryType, bool ignoreModTypes)
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
    else if (!ignoreModTypes && infantryType < Read_Infantry_Count())
    {
        infantryTypeString = GetNewInfantryIniName(infantryType);
    }
    else
    {
        Show_Error("Unable to convert infantry type to string: %d", infantryType);
    }

    return infantryTypeString;
}

UnitType Parse_Unit_Type(char* unitTypeString, bool* parseError, bool ignoreModTypes)
{
    if (Strings_Are_Equal(unitTypeString, "NONE"))
    {
        return UNIT_NONE;
    }
    else if (Strings_Are_Equal(unitTypeString, "HTNK"))
    {
        return UNIT_HTANK;
    }
    else if (Strings_Are_Equal(unitTypeString, "MTNK"))
    {
        return UNIT_MTANK;
    }
    else if (Strings_Are_Equal(unitTypeString, "LTNK"))
    {
        return UNIT_LTANK;
    }
    else if (Strings_Are_Equal(unitTypeString, "STNK"))
    {
        return UNIT_STANK;
    }
    else if (Strings_Are_Equal(unitTypeString, "FTNK"))
    {
        return UNIT_FTANK;
    }
    else if (Strings_Are_Equal(unitTypeString, "VICE"))
    {
        return UNIT_VICE;
    }
    else if (Strings_Are_Equal(unitTypeString, "APC"))
    {
        return UNIT_APC;
    }
    else if (Strings_Are_Equal(unitTypeString, "MSAM"))
    {
        return UNIT_MLRS;
    }
    else if (Strings_Are_Equal(unitTypeString, "JEEP"))
    {
        return UNIT_JEEP;
    }
    else if (Strings_Are_Equal(unitTypeString, "BGGY"))
    {
        return UNIT_BUGGY;
    }
    else if (Strings_Are_Equal(unitTypeString, "HARV"))
    {
        return UNIT_HARVESTER;
    }
    else if (Strings_Are_Equal(unitTypeString, "ARTY"))
    {
        return UNIT_ARTY;
    }
    else if (Strings_Are_Equal(unitTypeString, "MLRS"))
    {
        return UNIT_MSAM;
    }
    else if (Strings_Are_Equal(unitTypeString, "LST"))
    {
        return UNIT_HOVER;
    }
    else if (Strings_Are_Equal(unitTypeString, "MHQ"))
    {
        return UNIT_MHQ;
    }
    else if (Strings_Are_Equal(unitTypeString, "BOAT"))
    {
        return UNIT_GUNBOAT;
    }
    else if (Strings_Are_Equal(unitTypeString, "MCV"))
    {
        return UNIT_MCV;
    }
    else if (Strings_Are_Equal(unitTypeString, "BIKE"))
    {
        return UNIT_BIKE;
    }
    else if (Strings_Are_Equal(unitTypeString, "TRIC"))
    {
        return UNIT_TRIC;
    }
    else if (Strings_Are_Equal(unitTypeString, "TREX"))
    {
        return UNIT_TREX;
    }
    else if (Strings_Are_Equal(unitTypeString, "RAPT"))
    {
        return UNIT_RAPT;
    }
    else if (Strings_Are_Equal(unitTypeString, "STEG"))
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

    Show_Error("Unable to parse unit type from string: %s", unitTypeString);

    return UNIT_NONE;
}

UnitType Parse_Unit_Type(const char* unitTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(unitTypeString))
    {
        Show_Error("Unit type passed to Parse_Unit_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return UNIT_NONE;
    }

    auto unitTypeStr = strdup(unitTypeString);

    UnitType result = Parse_Unit_Type(unitTypeStr, parseError, ignoreModTypes);

    delete unitTypeStr;

    return result;
}

const char* Unit_Type_To_String(UnitType unitType, bool ignoreModTypes)
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
    else if (!ignoreModTypes && unitType < Read_Unit_Count())
    {
        unitTypeString = GetNewUnitIniName(unitType);
    }
    else
    {
        Show_Error("Unable to convert unit type to string: %d", unitType);
    }

    return unitTypeString;
}

SpeedType Parse_Unit_Speed_Type(char* unitSpeedTypeString, bool* parseError)
{
    SpeedType speedType = SPEED_NONE;

    if (Strings_Are_Equal(unitSpeedTypeString, "NONE"))
    {
        speedType = SPEED_NONE;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "FOOT"))
    {
        speedType = SPEED_FOOT;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "TRACK"))
    {
        speedType = SPEED_TRACK;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "HARVESTER"))
    {
        speedType = SPEED_HARVESTER;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "WHEEL"))
    {
        speedType = SPEED_WHEEL;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "WINGED"))
    {
        speedType = SPEED_WINGED;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "HOVER"))
    {
        speedType = SPEED_HOVER;
    }
    else if (Strings_Are_Equal(unitSpeedTypeString, "FLOAT"))
    {
        speedType = SPEED_FLOAT;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse unit speed type from string: %s", unitSpeedTypeString);
    }

    return speedType;
}

SpeedType Parse_Unit_Speed_Type(const char* unitSpeedTypeString, bool* parseError)
{
    if (String_Is_Empty(unitSpeedTypeString))
    {
        Show_Error("Unit speed type passed to Parse_Unit_Speed_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return SPEED_NONE;
    }

    auto unitSpeedTypeStr = strdup(unitSpeedTypeString);

    auto unitSpeedType = Parse_Unit_Speed_Type(unitSpeedTypeStr, parseError);

    delete unitSpeedTypeStr;

    return unitSpeedType;
}

const char* Unit_Speed_Type_To_String(SpeedType unitSpeedType)
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
        Show_Error("Unable to convert unit speed type to string: %d", unitSpeedType);
    }

    return unitSpeedTypeString;
}

AircraftType Parse_Aircraft_Type(char* aircraftTypeString, bool* parseError, bool ignoreModTypes)
{
    AircraftType aircraftType = AIRCRAFT_NONE;

    if (Strings_Are_Equal(aircraftTypeString, "NONE"))
    {
        aircraftType = AIRCRAFT_NONE;
    }
    else if (Strings_Are_Equal(aircraftTypeString, "A10"))
    {
        aircraftType = AIRCRAFT_A10;
    }
    else if (Strings_Are_Equal(aircraftTypeString, "C17"))
    {
        aircraftType = AIRCRAFT_CARGO;
    }
    else if (Strings_Are_Equal(aircraftTypeString, "HELI"))
    {
        aircraftType = AIRCRAFT_HELICOPTER;
    }
    else if (Strings_Are_Equal(aircraftTypeString, "ORCA"))
    {
        aircraftType = AIRCRAFT_ORCA;
    }
    else if (Strings_Are_Equal(aircraftTypeString, "TRAN"))
    {
        aircraftType = AIRCRAFT_TRANSPORT;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse aircraft type from string: %s", aircraftTypeString);
    }

    return aircraftType;
}

AircraftType Parse_Aircraft_Type(const char* aircraftTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(aircraftTypeString))
    {
        Show_Error("Aircraft type passed to Parse_Aircraft_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return AIRCRAFT_NONE;
    }

    auto aircraftTypeStr = strdup(aircraftTypeString);

    auto aircraftType = Parse_Aircraft_Type(aircraftTypeStr, parseError);

    delete aircraftTypeStr;

    return aircraftType;
}

const char* Aircraft_Type_To_String(AircraftType aircraftType, bool ignoreModTypes)
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
    else
    {
        Show_Error("Unable to convert aircraft type to string: %d", aircraftType);
    }

    return aircraftTypeString;
}

StructType Parse_Structure_Type(char* structTypeString, bool* parseError, bool ignoreModTypes)
{
    StructType structType;

    if (Strings_Are_Equal(structTypeString, "NONE"))
    {
        structType = STRUCT_NONE;
    }
    else if (Strings_Are_Equal(structTypeString, "WEAP"))
    {
        structType = STRUCT_WEAP;
    }
    else if (Strings_Are_Equal(structTypeString, "GTWR"))
    {
        structType = STRUCT_GTOWER;
    }
    else if (Strings_Are_Equal(structTypeString, "ATWR"))
    {
        structType = STRUCT_ATOWER;
    }
    else if (Strings_Are_Equal(structTypeString, "OBLI"))
    {
        structType = STRUCT_OBELISK;
    }
    else if (Strings_Are_Equal(structTypeString, "HQ"))
    {
        structType = STRUCT_RADAR;
    }
    else if (Strings_Are_Equal(structTypeString, "GUN"))
    {
        structType = STRUCT_TURRET;
    }
    else if (Strings_Are_Equal(structTypeString, "FACT"))
    {
        structType = STRUCT_CONST;
    }
    else if (Strings_Are_Equal(structTypeString, "PROC"))
    {
        structType = STRUCT_REFINERY;
    }
    else if (Strings_Are_Equal(structTypeString, "SILO"))
    {
        structType = STRUCT_STORAGE;
    }
    else if (Strings_Are_Equal(structTypeString, "HPAD"))
    {
        structType = STRUCT_HELIPAD;
    }
    else if (Strings_Are_Equal(structTypeString, "SAM"))
    {
        structType = STRUCT_SAM;
    }
    else if (Strings_Are_Equal(structTypeString, "AFLD"))
    {
        structType = STRUCT_AIRSTRIP;
    }
    else if (Strings_Are_Equal(structTypeString, "NUKE"))
    {
        structType = STRUCT_POWER;
    }
    else if (Strings_Are_Equal(structTypeString, "NUK2"))
    {
        structType = STRUCT_ADVANCED_POWER;
    }
    else if (Strings_Are_Equal(structTypeString, "HOSP"))
    {
        structType = STRUCT_HOSPITAL;
    }
    else if (Strings_Are_Equal(structTypeString, "PYLE"))
    {
        structType = STRUCT_BARRACKS;
    }
    else if (Strings_Are_Equal(structTypeString, "ARCO"))
    {
        structType = STRUCT_TANKER;
    }
    else if (Strings_Are_Equal(structTypeString, "FIX"))
    {
        structType = STRUCT_REPAIR;
    }
    else if (Strings_Are_Equal(structTypeString, "BIO"))
    {
        structType = STRUCT_BIO_LAB;
    }
    else if (Strings_Are_Equal(structTypeString, "HAND"))
    {
        structType = STRUCT_HAND;
    }
    else if (Strings_Are_Equal(structTypeString, "TMPL"))
    {
        structType = STRUCT_TEMPLE;
    }
    else if (Strings_Are_Equal(structTypeString, "EYE"))
    {
        structType = STRUCT_EYE;
    }
    else if (Strings_Are_Equal(structTypeString, "MISS"))
    {
        structType = STRUCT_MISSION;
    }
    else if (Strings_Are_Equal(structTypeString, "V01"))
    {
        structType = STRUCT_V01;
    }
    else if (Strings_Are_Equal(structTypeString, "V02"))
    {
        structType = STRUCT_V02;
    }
    else if (Strings_Are_Equal(structTypeString, "V03"))
    {
        structType = STRUCT_V03;
    }
    else if (Strings_Are_Equal(structTypeString, "V04"))
    {
        structType = STRUCT_V04;
    }
    else if (Strings_Are_Equal(structTypeString, "V05"))
    {
        structType = STRUCT_V05;
    }
    else if (Strings_Are_Equal(structTypeString, "V06"))
    {
        structType = STRUCT_V06;
    }
    else if (Strings_Are_Equal(structTypeString, "V07"))
    {
        structType = STRUCT_V07;
    }
    else if (Strings_Are_Equal(structTypeString, "V08"))
    {
        structType = STRUCT_V08;
    }
    else if (Strings_Are_Equal(structTypeString, "V09"))
    {
        structType = STRUCT_V09;
    }
    else if (Strings_Are_Equal(structTypeString, "V10"))
    {
        structType = STRUCT_V10;
    }
    else if (Strings_Are_Equal(structTypeString, "V11"))
    {
        structType = STRUCT_V11;
    }
    else if (Strings_Are_Equal(structTypeString, "V12"))
    {
        structType = STRUCT_V12;
    }
    else if (Strings_Are_Equal(structTypeString, "V13"))
    {
        structType = STRUCT_V13;
    }
    else if (Strings_Are_Equal(structTypeString, "V14"))
    {
        structType = STRUCT_V14;
    }
    else if (Strings_Are_Equal(structTypeString, "V15"))
    {
        structType = STRUCT_V15;
    }
    else if (Strings_Are_Equal(structTypeString, "V16"))
    {
        structType = STRUCT_V16;
    }
    else if (Strings_Are_Equal(structTypeString, "V17"))
    {
        structType = STRUCT_V17;
    }
    else if (Strings_Are_Equal(structTypeString, "V18"))
    {
        structType = STRUCT_V18;
    }
    else if (Strings_Are_Equal(structTypeString, "V19"))
    {
        structType = STRUCT_PUMP;
    }
    else if (Strings_Are_Equal(structTypeString, "V20"))
    {
        structType = STRUCT_V20;
    }
    else if (Strings_Are_Equal(structTypeString, "V21"))
    {
        structType = STRUCT_V21;
    }
    else if (Strings_Are_Equal(structTypeString, "V22"))
    {
        structType = STRUCT_V22;
    }
    else if (Strings_Are_Equal(structTypeString, "V23"))
    {
        structType = STRUCT_V23;
    }
    else if (Strings_Are_Equal(structTypeString, "V24"))
    {
        structType = STRUCT_V24;
    }
    else if (Strings_Are_Equal(structTypeString, "V25"))
    {
        structType = STRUCT_V25;
    }
    else if (Strings_Are_Equal(structTypeString, "V26"))
    {
        structType = STRUCT_V26;
    }
    else if (Strings_Are_Equal(structTypeString, "V27"))
    {
        structType = STRUCT_V27;
    }
    else if (Strings_Are_Equal(structTypeString, "V28"))
    {
        structType = STRUCT_V28;
    }
    else if (Strings_Are_Equal(structTypeString, "V29"))
    {
        structType = STRUCT_V29;
    }
    else if (Strings_Are_Equal(structTypeString, "V30"))
    {
        structType = STRUCT_V30;
    }
    else if (Strings_Are_Equal(structTypeString, "V31"))
    {
        structType = STRUCT_V31;
    }
    else if (Strings_Are_Equal(structTypeString, "V32"))
    {
        structType = STRUCT_V32;
    }
    else if (Strings_Are_Equal(structTypeString, "V33"))
    {
        structType = STRUCT_V33;
    }
    else if (Strings_Are_Equal(structTypeString, "V34"))
    {
        structType = STRUCT_V34;
    }
    else if (Strings_Are_Equal(structTypeString, "V35"))
    {
        structType = STRUCT_V35;
    }
    else if (Strings_Are_Equal(structTypeString, "V36"))
    {
        structType = STRUCT_V36;
    }
    else if (Strings_Are_Equal(structTypeString, "V37"))
    {
        structType = STRUCT_V37;
    }
    else if (Strings_Are_Equal(structTypeString, "SBAG"))
    {
        structType = STRUCT_SANDBAG_WALL;
    }
    else if (Strings_Are_Equal(structTypeString, "CYCL"))
    {
        structType = STRUCT_CYCLONE_WALL;
    }
    else if (Strings_Are_Equal(structTypeString, "BRIK"))
    {
        structType = STRUCT_BRICK_WALL;
    }
    else if (Strings_Are_Equal(structTypeString, "BARB"))
    {
        structType = STRUCT_BARBWIRE_WALL;
    }
    else if (Strings_Are_Equal(structTypeString, "WOOD"))
    {
        structType = STRUCT_WOOD_WALL;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse structure from string: %s", structTypeString);
    }

    return structType;
}

StructType Parse_Structure_Type(const char* structTypeString, bool* parseError, bool ignoreModTypes)
{
    if (String_Is_Empty(structTypeString))
    {
        Show_Error("Structure type passed to Parse_Structure_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return STRUCT_NONE;
    }

    auto structTypeStr = strdup(structTypeString);

    auto structType = Parse_Structure_Type(structTypeStr, parseError);

    delete structTypeStr;

    return structType;
}

const char* Structure_Type_To_String(StructType structType, bool ignoreModTypes)
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
    else
    {
        Show_Error("Unable to convert structure type to string: %d", structType);
    }

    return structTypeString;
}

long Structure_Type_To_Prerequisite(StructType structType, bool* parseError)
{
    if (structType == STRUCT_NONE)
    {
        return STRUCTF_NONE;
    }

    if (structType < STRUCT_FIRST || structType > STRUCT_COUNT - 1)
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        return STRUCTF_NONE;
    }

    return 1L << structType;
}

FactoryType Parse_Factory_Type(char* factoryTypeString, bool* parseError)
{
    FactoryType factoryType;

    if (Strings_Are_Equal(factoryTypeString, "NONE"))
    {
        factoryType = FACTORY_TYPE_NONE;
    }
    else if (Strings_Are_Equal(factoryTypeString, "INFANTRY"))
    {
        factoryType = FACTORY_TYPE_INFANTRY;
    }
    else if (Strings_Are_Equal(factoryTypeString, "UNIT"))
    {
        factoryType = FACTORY_TYPE_UNIT;
    }
    else if (Strings_Are_Equal(factoryTypeString, "AIRCRAFT"))
    {
        factoryType = FACTORY_TYPE_AIRCRAFT;
    }
    else if (Strings_Are_Equal(factoryTypeString, "BUILDING"))
    {
        factoryType = FACTORY_TYPE_BUILDING;
    }
    else
    {
        if (parseError != NULL)
        {
            *parseError = true;
        }

        Show_Error("Unable to parse factory type from string: %s", factoryTypeString);
    }

    return factoryType;
}

FactoryType Parse_Factory_Type(const char* factoryTypeString, bool* parseError)
{
    if (String_Is_Empty(factoryTypeString))
    {
        Show_Error("Factory type passed to Parse_Factory_Type was null or empty");

        if (parseError != NULL)
        {
            *parseError = true;
        }

        return FACTORY_TYPE_NONE;
    }

    auto factoryTypeStr = strdup(factoryTypeString);

    auto factoryType = Parse_Factory_Type(factoryTypeStr, parseError);

    delete factoryTypeStr;

    return factoryType;
}

const char* Factory_Type_To_String(FactoryType factoryType)
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
        Show_Error("Unable to convert factory type to string: %d", factoryType);
    }

    return factoryTypeString;
}
