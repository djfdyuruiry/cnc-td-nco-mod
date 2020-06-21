#include "function.h"

HousesType Parse_House_Type(char* houseTypeString)
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
        Show_Error("Unable to parse house from string: %s", houseTypeString);
    }

	return house;
}

char* Parse_House_Type(HousesType houseType)
{
    char* houseTypeString;

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

StructType Parse_Structure_Type(char* structTypeString)
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
        Show_Error("Unable to parse structure from string: %s", structTypeString);
    }

	return structType;
}

char* Structure_Type_To_String(StructType structType)
{
    char* structTypeString;
 
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

WeaponType Parse_Weapon_Type(char* weaponTypeString)
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
    else if (Strings_Are_Equal(weaponTypeString, "MLRS"))
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
    else if (Strings_Are_Equal(weaponTypeString, "ATWR"))
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
    else if (Strings_Are_Equal(weaponTypeString, "STEG"))
    {
        weaponType = WEAPON_STEG;
    }
    else if (Strings_Are_Equal(weaponTypeString, "TREX"))
    {
        weaponType = WEAPON_TREX;
    }
    else
    {
        Show_Error("Unable to parse weapon type from string: %s", weaponTypeString);
    }

    return weaponType;
}

char* Weapon_Type_To_String(WeaponType weaponType)
{
    char* weaponTypeString;

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
        weaponTypeString = "MLRS";
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
        weaponTypeString = "ATWR";
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
        weaponTypeString = "STEG";
    }
    else if (weaponType == WEAPON_TREX)
    {
        weaponTypeString = "TREX";
    }
    else
    {
        Show_Error("Unable to convert weapon type to string: %d", weaponType);
    }

    return weaponTypeString;
}
