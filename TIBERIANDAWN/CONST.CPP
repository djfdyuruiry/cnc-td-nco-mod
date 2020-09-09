//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header:   F:\projects\c&c\vcs\code\const.cpv   2.17   16 Oct 1995 16:52:24   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CONST.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 20, 1993                                           *
 *                                                                                             *
 *                  Last Update : September 20, 1993   [JLB]                                   *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "function.h"


char const * SourceName[SOURCE_COUNT] =
{
	"North",
	"East",
	"South",
	"West",
	"Shipping",
	"Beach",
	"Air",
	"Visible",
	"EnemyBase",
	"HomeBase",
	"Ocean",
};


/***************************************************************************
**	Relative coordinate offsets from the center of a cell for each
**	of the legal positions that an object in a cell may stop at. Only infantry
**	are allowed to stop at other than the center of the cell.
*/
COORDINATE const StoppingCoordAbs[5] = {
	0x00800080L,	// center
	0x00400040L,	// upper left
	0x004000C0L,	// upper right
	0x00C00040L,	// lower left
	0x00C000C0L		// lower right
};

/***************************************************************************
**	These are the various weapons and their characteristics.
*/

static WeaponTypeClass const WEP_RIFLE("Sniper Rifle", "RIFLE", WEAPON_RIFLE, BULLET_SNIPER, 125, 40, 0x0580, VOC_SNIPER, ANIM_NONE);
static WeaponTypeClass const WEP_CHAIN_GUN("Chain Gun", "CHAIN_GUN", WEAPON_CHAIN_GUN, BULLET_SPREADFIRE, 25, 50, 0x0400, VOC_MINI, ANIM_GUN_N);
static WeaponTypeClass const WEP_PISTOL("Pistol", "PISTOL", WEAPON_PISTOL, BULLET_BULLET, 1, 7, 0x01C0, VOC_RIFLE, ANIM_NONE);
static WeaponTypeClass const WEP_M16("Assault Rifle", "M16", WEAPON_M16, BULLET_BULLET, 15, 20, 0x0200, VOC_MGUN2, ANIM_NONE);
static WeaponTypeClass const WEP_DRAGON("Bazooka", "DRAGON", WEAPON_DRAGON, BULLET_TOW, 30, 60, 0x0400, VOC_BAZOOKA, ANIM_NONE);
static WeaponTypeClass const WEP_FLAMETHROWER("Flamethrower Blast", "FLAMETHROWER", WEAPON_FLAMETHROWER, BULLET_FLAME, 35, 50, 0x0200, VOC_FLAMER1, ANIM_FLAME_N);
static WeaponTypeClass const WEP_FLAME_TONGUE("Flame Tank Blast", "FLAME_TONGUE", WEAPON_FLAME_TONGUE, BULLET_FLAME, 50, 50, 0x0200, VOC_FLAMER1, ANIM_FLAME_N);
static WeaponTypeClass const WEP_CHEMSPRAY("Chemical Spray", "CHEMSPRAY", WEAPON_CHEMSPRAY, BULLET_CHEMSPRAY, 80, 70, 0x0200, VOC_FLAMER1, ANIM_CHEM_N);
static WeaponTypeClass const WEP_GRENADE("Grenade", "GRENADE", WEAPON_GRENADE, BULLET_GRENADE, 50, 60, 0x0340, VOC_TOSS, ANIM_NONE);
static WeaponTypeClass const WEP_75MM("Light Tank", "75MM", WEAPON_75MM, BULLET_APDS, 25, 60, 0x0400, VOC_TANK2, ANIM_MUZZLE_FLASH);
static WeaponTypeClass const WEP_105MM("Medium Tank", "105MM", WEAPON_105MM, BULLET_APDS, 30, 50, 0x04C0, VOC_TANK3, ANIM_MUZZLE_FLASH);
static WeaponTypeClass const WEP_120MM("Mammoth Tank", "120MM", WEAPON_120MM, BULLET_APDS, 40, 80, 0x04C0, VOC_TANK4, ANIM_MUZZLE_FLASH);
static WeaponTypeClass const WEP_TURRET_GUN("Gun Turret", "TURRET_GUN", WEAPON_TURRET_GUN, BULLET_APDS, 40, 60, 0x0600, VOC_TANK4, ANIM_MUZZLE_FLASH);
static WeaponTypeClass const WEP_MAMMOTH_TUSK("Mammoth Tank Anti-Infantry Missile", "MAMMOTH_TUSK", WEAPON_MAMMOTH_TUSK, BULLET_SSM, 75, 80, 0x0500, VOC_ROCKET1, ANIM_NONE);
static WeaponTypeClass const WEP_MLRS("Rocket Launcher", "ROCKET_LAUNCHER", WEAPON_MLRS, BULLET_SSM2, 75, 80, 0x0600, VOC_ROCKET1, ANIM_NONE);
static WeaponTypeClass const WEP_155MM("Artillery", "155MM", WEAPON_155MM, BULLET_HE, 150, 65, 0x0600, VOC_TANK1, ANIM_MUZZLE_FLASH);
static WeaponTypeClass const WEP_M60MG("APC Gun", "M60MG", WEAPON_M60MG, BULLET_BULLET, 15, 30, 0x0400, VOC_MGUN11, ANIM_GUN_N);
static WeaponTypeClass const WEP_TOMAHAWK("Gunboat", "TOMAHAWK", WEAPON_TOMAHAWK, BULLET_SSM, 60, 35, 0x0780, VOC_ROCKET2, ANIM_NONE);
static WeaponTypeClass const WEP_TOW_TWO("Advanced Guard Tower", "TOWTWO", WEAPON_TOW_TWO, BULLET_SSM, 60, 40, 0x0680, VOC_ROCKET2, ANIM_NONE);
static WeaponTypeClass const WEP_NAPALM("Napalm", "NAPALM", WEAPON_NAPALM, BULLET_NAPALM, 100, 20, 0x0480, VOC_NONE, ANIM_NONE);
static WeaponTypeClass const WEP_OBELISK_LASER("Laser", "OBELISK_LASER", WEAPON_OBELISK_LASER, BULLET_LASER, 200, 90, 0x0780, VOC_LASER, ANIM_NONE);
static WeaponTypeClass const WEP_NIKE("SAM Site", "NIKE", WEAPON_NIKE, BULLET_SAM, 50, 50, 0x0780, VOC_ROCKET2, ANIM_NONE);
static WeaponTypeClass const WEP_HONEST_JOHN("SSM Launcher", "HONEST_JOHN", WEAPON_HONEST_JOHN, BULLET_HONEST_JOHN, 100, 200, 0x0A00, VOC_ROCKET1, ANIM_NONE);
static WeaponTypeClass const WEP_STEG("Dino Ram", "DINO_RAM", WEAPON_STEG, BULLET_HEADBUTT, 100, 30, 0x0180, VOC_DINOATK1, ANIM_NONE);
static WeaponTypeClass const WEP_TREX("Dino Bite", "DINO_BITE", WEAPON_TREX, BULLET_TREXBITE, 155, 30, 0x0180, VOC_DINOATK1, ANIM_NONE);

WeaponTypeClass const** WeaponTypeClass::Pointers = NULL;

WeaponTypeClass::WeaponTypeClass(
	const char* friendlyName,
	char const* ininame,
	WeaponType owner,
	BulletType fires,
	unsigned char attack,
	unsigned char rof,
	int range,
	VocType sound,
	AnimType anim
) : ObjectTypeClass(true, false, false, true, false, false, true, true, TXT_NONE, ininame, ARMOR_NONE, 0, false)
{
	Sound = sound;
	Anim = anim;

	auto weaponTypeStr = Weapon_Type_To_String(owner);
	//strncpy((char*)IniName, ininame, sizeof(IniName));
	//((char&)IniName[sizeof(IniName) - 1]) = '\0';
	IniName = strdup(ininame);
	GetRules() << RulesIniSectionBuilder::BuildWeaponSection(weaponTypeStr);

	FriendlyName = ReadRuleValue<char*>(weaponTypeStr, FRIENDLY_NAME_RULE, strdup(friendlyName));
	Fires = ReadRuleValue<BulletType>(weaponTypeStr, WEAPON_FIRES_RULE, fires);
	Attack = Read_Weapon_Damage(weaponTypeStr, attack);
	ROF = Read_Weapon_Rate_Of_Fire(weaponTypeStr, rof);
	Range = Read_Weapon_Range(weaponTypeStr, range);
	IsModType = false;

	Read_Weapon_Rules(this);
}

WeaponTypeClass const* WeaponTypeClass::By_Type(WeaponType type)
{
	if (Pointers != NULL)
	{
		Log_Trace("Weapon lookup by type: %d", type);
		return Pointers[type];
	}
	auto weaponCount = Read_Weapon_Count();
	Pointers = new WeaponTypeClass const* [weaponCount] {
		&WEP_RIFLE,
			&WEP_CHAIN_GUN,
			&WEP_PISTOL,
			&WEP_M16,
			&WEP_DRAGON,
			&WEP_FLAMETHROWER,
			&WEP_FLAME_TONGUE,
			&WEP_CHEMSPRAY,
			&WEP_GRENADE,
			&WEP_75MM,
			&WEP_105MM,
			&WEP_120MM,
			&WEP_TURRET_GUN,
			&WEP_MAMMOTH_TUSK,
			&WEP_MLRS,
			&WEP_155MM,
			&WEP_M60MG,
			&WEP_TOMAHAWK,
			&WEP_TOW_TWO,
			&WEP_NAPALM,
			&WEP_OBELISK_LASER,
			&WEP_NIKE,
			&WEP_HONEST_JOHN,
			&WEP_STEG,
			&WEP_TREX,
	};
	InitialiseModsIfRequired();

	return By_Type(type);
}

void WeaponTypeClass::Read_Weapon_Rules(WeaponTypeClass* weapon)
{
	auto iniName = weapon->IniName;

	//Read_Object_Rules(weapon);
	//Read_Techno_Rules(weapon);
	weapon->Fires = ReadRuleValue<BulletType>(iniName, WEAPON_FIRES_RULE, weapon->Fires);
	weapon->Attack = Read_Weapon_Damage(iniName, weapon->Attack);
	weapon->ROF = Read_Weapon_Rate_Of_Fire(iniName, weapon->ROF);
	weapon->Range = Read_Weapon_Range(iniName, weapon->Range);
}

void WeaponTypeClass::Add_Weapon_Type(WeaponTypeClass* WeaponInstance)
{
	Pointers[WeaponInstance->Type] = WeaponInstance;
}

void WeaponTypeClass::One_Time(void)
{
	return;
}

char* WeaponTypeClass::Resolve_Ini_Name()
{
	return IsModType
		? ModBaseIniName
		: IniName;
}
/***************************************************************************
**	These are the various warheads.
**
** spread factor, destroys walls, destroys wood, destroys Tiberium, {armor defense table}
**           -vs- {none, wood, aluminum, steel, concrete}
*/
static WarheadTypeClass const WH_SA("Small Arms", "SA", WARHEAD_SA, 2, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x80, 0x90, 0x40, 0x40});
static WarheadTypeClass const WH_HE("High Explosive", "HE", WARHEAD_HE, 6, true, true, true, new unsigned[ARMOR_COUNT] {0xE0, 0xC0, 0x90, 0x40, 0x100});
static WarheadTypeClass const WH_AP("Armor Piercing", "AP", WARHEAD_AP, 6, true, true, false, new unsigned[ARMOR_COUNT] {0x40, 0xC0, 0xC0, 0x100, 0x80});
static WarheadTypeClass const WH_FIRE("Fire", "FIRE", WARHEAD_FIRE, 8, false, true, true, new unsigned[ARMOR_COUNT] {0xE0, 0x100, 0xB0, 0x40, 0x80});
static WarheadTypeClass const WH_LASER("Laser", "LASER", WARHEAD_LASER, 4, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x100, 0x100, 0x100, 0x100});
static WarheadTypeClass const WH_PB("Ion Cannon Beam", "PB", WARHEAD_PB, 7, true, true, true, new unsigned[ARMOR_COUNT] {0x100, 0x100, 0xC0, 0xC0, 0xC0});
static WarheadTypeClass const WH_FIST("Punch", "FIST", WARHEAD_FIST, 4, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x20, 0x20, 0x10, 0x10});
static WarheadTypeClass const WH_FOOT("Kick", "FOOT", WARHEAD_FOOT, 4, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x20, 0x20, 0x10, 0x10});
static WarheadTypeClass const WH_HOLLOW_POINT("Hollow Point", "HOLLOW_POINT", WARHEAD_HOLLOW_POINT, 4, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x08, 0x08, 0x08, 0x08});
static WarheadTypeClass const WH_SPORE("Tiberium Spore", "SPORE", WARHEAD_SPORE, 255, false, false, false, new unsigned[ARMOR_COUNT] {0x100, 0x01, 0x01, 0x01, 0x01});
static WarheadTypeClass const WH_HEADBUTT("Stegosaurus Headbutt", "HEADBUTT", WARHEAD_HEADBUTT, 1, true, true, false, new unsigned[ARMOR_COUNT] {0x100, 0xC0, 0x80, 0x20, 0x08});
static WarheadTypeClass const WH_FEEDME("T-Rex Teeth", "FEEDME", WARHEAD_FEEDME, 1, true, true, false, new unsigned[ARMOR_COUNT] {0x100, 0xC0, 0x80, 0x20, 0x08});
WarheadTypeClass const** WarheadTypeClass::Pointers = NULL;

WarheadTypeClass::WarheadTypeClass(
	const char* friendlyName,
	char const* ininame,
	WarheadType type,
	int spreadFactor,
	bool isWallDestroyer,
	bool isWoodDestroyer,
	bool isTiberiumDestroyer,
	unsigned armorModifiers[ARMOR_COUNT]
) : ObjectTypeClass(true, false, false, true, false, false, true, true, TXT_NONE, ininame, ARMOR_NONE, 0, false)
{
	Modifier = armorModifiers;

	auto warheadTypeString = Warhead_Type_To_String(type);
	IniName = strdup(ininame);
	//strncpy((char*)IniName, ininame, sizeof(IniName));
	//((char&)IniName[sizeof(IniName) - 1]) = '\0';
	GetRules() << RulesIniSectionBuilder::BuildWarheadSection(warheadTypeString);

	FriendlyName = ReadRuleValue<char*>(warheadTypeString, FRIENDLY_NAME_RULE, strdup(friendlyName));
	SpreadFactor = ReadRuleValue<unsigned int>(warheadTypeString, WARHEAD_SPREAD_FACTOR_RULE, spreadFactor);
	IsWallDestroyer = ReadRuleValue<bool>(warheadTypeString, WARHEAD_DESTORY_WALLS_RULE, isWallDestroyer);
	IsWoodDestroyer = ReadRuleValue<bool>(warheadTypeString, WARHEAD_DESTORY_WOOD_RULE, isWoodDestroyer);
	IsTiberiumDestroyer = ReadRuleValue<bool>(warheadTypeString, WARHEAD_DESTORY_TIBERIUM_RULE, isTiberiumDestroyer);

	for (auto armorType = ARMOR_FIRST; armorType < ARMOR_COUNT; armorType++)
	{
		auto modifierRule = Map_Armor_Modifier_To_Warhead_Rule(armorType);

		Modifier[armorType] = ReadRuleValue<unsigned int>(warheadTypeString, modifierRule, Modifier[armorType]);
	}
	IsModType = false;
	Read_Warhead_Rules(this);
}
void WarheadTypeClass::Read_Warhead_Rules(WarheadTypeClass* warhead)
{
	auto iniName = warhead->IniName;

	//Read_Object_Rules(warhead);
	//Read_Techno_Rules(warhead);

	warhead->FriendlyName = ReadRuleValue<char*>(iniName, FRIENDLY_NAME_RULE, warhead->FriendlyName);
	warhead->SpreadFactor = ReadRuleValue<unsigned int>(iniName, WARHEAD_SPREAD_FACTOR_RULE, warhead->SpreadFactor);
	warhead->IsWallDestroyer = ReadRuleValue<bool>(iniName, WARHEAD_DESTORY_WALLS_RULE, warhead->IsWallDestroyer);
	warhead->IsWoodDestroyer = ReadRuleValue<bool>(iniName, WARHEAD_DESTORY_WOOD_RULE, warhead->IsWoodDestroyer);
	warhead->IsTiberiumDestroyer = ReadRuleValue<bool>(iniName, WARHEAD_DESTORY_TIBERIUM_RULE, warhead->IsTiberiumDestroyer);
}

void WarheadTypeClass::Add_Warhead_Type(WarheadTypeClass* WarheadInstance)
{
	Pointers[WarheadInstance->Type] = WarheadInstance;
}

WarheadTypeClass const* WarheadTypeClass::By_Type(WarheadType type)
{
	if (Pointers != NULL)
	{
		Log_Trace("Warhead lookup by type: %d", type);
		return Pointers[type];
	}

	auto warheadCount = Read_Warhead_Count();
	Pointers = new WarheadTypeClass const* [warheadCount] {
		&WH_SA,
			& WH_HE,
			& WH_AP,
			& WH_FIRE,
			& WH_LASER,
			& WH_PB,
			& WH_FIST,
			& WH_FOOT,
			& WH_HOLLOW_POINT,
			& WH_SPORE,
			& WH_HEADBUTT,
			& WH_FEEDME,
	};
	InitialiseModsIfRequired();

	return By_Type(type);
}

void WarheadTypeClass::One_Time(void)
{
	return;
}

char* WarheadTypeClass::Resolve_Ini_Name()
{
	return IsModType
		? ModBaseIniName
		: IniName;
}

/***************************************************************************
**	Converts pixel values (cell relative) into the appropriate lepton (sub cell)
**	value. This is used to convert pixel (screen) coordinates into the underlying
**	coordinate system.
*/
unsigned char const Pixel2Lepton[24] = {
	0x00,0x0B,0x15,0x20,0x2B,0x35,0x40,0x4B,
	0x55,0x60,0x6B,0x75,0x80,0x8B,0x95,0xA0,
	0xAB,0xB5,0xC0,0xCB,0xD5,0xE0,0xEB,0xF5
};


/***************************************************************************
**	This array is used to index a facing in order to retrieve a cell
**	offset that, when added to another cell, will achieve the adjacent cell
**	in the indexed direction.
*/
CELL const AdjacentCell[FACING_COUNT] = {
	-(MAP_CELL_W),				// North
	-(MAP_CELL_W-1),			// North East
	1,								// East
	MAP_CELL_W+1,				// South East
	MAP_CELL_W,					// South
	MAP_CELL_W-1,				// South West
	-1,							// West
	-(MAP_CELL_W+1)			// North West
};

COORDINATE const AdjacentCoord[FACING_COUNT] = {
	0xFF000000L,
	0xFF000100L,
	0x00000100L,
	0x01000100L,
	0x01000000L,
	0x0100FF00L,
	0x0000FF00L,
	0xFF00FF00L
};


/***************************************************************************
** This converts 0..255 facing values into either 8, 16, or 32 facing values.
**	Note: a simple shift won't suffice because 0..255 facing values should
**	be converted to the CLOSEST appropriate facing, NOT rounded down to the
**	nearest facing.
*/
unsigned char const Facing8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#ifdef NEVER
unsigned char const Facing16[256] = {
	0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
	12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,
	14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0
};
#endif

/*
**	This table incorporates a compensating factor for the distortion caused
**	by 3D-Studio when it tries to render 45% angles.
*/
unsigned char const Facing32[256] = {
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,
	3,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,
	8,8,8,9,9,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,
	13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,
	16,16,16,16,16,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,
	19,20,20,20,20,20,20,21,21,21,21,21,21,21,22,22,22,22,22,22,22,23,23,23,23,23,23,23,24,24,24,24,
	24,24,24,25,25,25,25,25,25,25,26,26,26,26,26,26,26,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,
	29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,0,0,0,0,0,0
};

#ifdef OBSOLETE
unsigned char const Facing32[256] = {
	0,0,0,0,
	1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,
	4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,
	9,9,9,9,9,9,9,9,
	10,10,10,10,10,10,10,10,
	11,11,11,11,11,11,11,11,
	12,12,12,12,12,12,12,12,
	13,13,13,13,13,13,13,13,
	14,14,14,14,14,14,14,14,
	15,15,15,15,15,15,15,15,
	16,16,16,16,16,16,16,16,
	17,17,17,17,17,17,17,17,
	18,18,18,18,18,18,18,18,
	19,19,19,19,19,19,19,19,
	20,20,20,20,20,20,20,20,
	21,21,21,21,21,21,21,21,
	22,22,22,22,22,22,22,22,
	23,23,23,23,23,23,23,23,
	24,24,24,24,24,24,24,24,
	25,25,25,25,25,25,25,25,
	26,26,26,26,26,26,26,26,
	27,27,27,27,27,27,27,27,
	28,28,28,28,28,28,28,28,
	29,29,29,29,29,29,29,29,
	30,30,30,30,30,30,30,30,
	31,31,31,31,31,31,31,31,
	0,0,0,0
};
#endif


/***************************************************************************
**	These are the movement costs (in ticks at fastest speed) to enter each
**	of the given terrain cells.
*/
#define	S1	0x00
#define	S2 0x40
#define	S3 0x70
#define	S4 0xA0
#define	S5 0xC0
#define	S6 0xFF
GroundType const Ground[LAND_COUNT] = {
//						Foot
//						|		Tracked
//						|		|		Harvester
//						|		|		|		Wheeled
//						|		|		|		|		Winged
//						|		|		|		|		|		Hover
//						|		|		|		|		|		|		float  build
	{66, 				{S3,	S3,	S3,	S4,	S6,	S5,	S1	}, true},	// LAND_CLEAR
	{68,				{S5,	S4,	S4,	S4,  	S6,	S5,	S1	}, true},	// LAND_ROAD
	{BLUE,			{S1,	S1,	S1,	S1,	S6,	S5,	S6	}, false},	// LAND_WATER
	{DKGREY,			{S1,	S1,	S1,	S1,	S6,	S1,	S1	}, false},	// LAND_ROCK
	{DKGREY,			{S1,	S1,	S1,	S1,	S6,	S1,	S1	}, false},	// LAND_WALL
	{143,				{S3,	S3,	S3,	S4,	S6,	S5,	S1	}, false},	// LAND_TIBERIUM
	{66,				{S3,	S3,	S3,	S4,	S6,	S5,	S1	}, false},	// LAND_BEACH
};


/***************************************************************************
**	These are the names of the theaters.
*/
TheaterDataType const Theaters[THEATER_COUNT] = {
	{"DESERT","DESERT","DES"},
	{"JUNGLE","JUNGLE","JUN"},
	{"TEMPERATE","TEMPERAT","TEM"},
	{"WINTER","WINTER","WIN"}
};


/***************************************************************************
** These are the remap tables that are used to convert the units/buildings
**	into the other color schemes.
*/
unsigned char const RemapGold[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapRed[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	127,126,125,124,122,46,120,47,125,124,123,122,42,121,120,120,		// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapBlue[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	2,119,118,135,136,138,112,12,118,135,136,137,138,139,114,112,		// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapOrange[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	24,25,26,27,29,31,46,47,26,27,28,29,30,31,43,47,						// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapGreen[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	5,165,166,167,159,142,140,199,166,167,157,3,159,143,142,141,		// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapLtBlue[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	161,200,201,202,204,205,206,12,201,202,203,204,205,115,198,114,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

unsigned char const RemapNone[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,										// 0..15
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,						// 16..31
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,						// 32..47
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,						// 48..63
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,						// 64..79
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,						// 80..95
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,		// 96..111
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,	// 112..127
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,	// 128..143
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,	// 144..159
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,	// 160..175
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,	// 176..191
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,	// 192..207
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,	// 208..223
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,	// 224..239
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255	// 240..255
};

const FacingType FacingCardinals[4]{ FACING_N, FACING_E, FACING_S, FACING_W };
