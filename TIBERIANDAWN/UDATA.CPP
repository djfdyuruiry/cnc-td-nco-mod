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

/* $Header:   F:\projects\c&c\vcs\code\udata.cpv   2.17   16 Oct 1995 16:50:42   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : UDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : June 26, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.      *
 *   UnitTypeClass::Create_And_Place -- Creates and places a unit object onto the map.         *
 *   UnitTypeClass::Create_One_Of -- Creates a unit in limbo.                                  *
 *   UnitTypeClass::Dimensions -- Determines the unit's pixel dimensions.                      *
 *   UnitTypeClass::Display -- Displays a generic unit shape.                                  *
 *   UnitTypeClass::From_Name -- Fetch class pointer from specified name.                      *
 *   UnitTypeClass::Max_Pips -- Fetches the maximum pips allowed for this unit.                *
 *   UnitTypeClass::Occupy_List -- Returns with unit occupation list.                          *
 *   UnitTypeClass::One_Time -- Performs one time processing for unit type class objects.      *
 *   UnitTypeClass::Prep_For_Add -- Prepares scenario editor to add unit.                      *
 *   UnitTypeClass::UnitTypeClass -- Constructor for unit types.                               *
 *   UnitTypeClass::Who_Can_Build_Me -- Determines which factory can build this unit type.     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

void const * UnitTypeClass::WakeShapes = 0;

// Visceroid
static UnitTypeClass const UnitVisceroid(
	UNIT_VICE,
	TXT_VISCEROID,							// NAME:			Text name of this unit type.
	TXT_VISCEROID_STRING,
	"VICE",									// NAME:			Text name of this unit type.
	ANIM_NAPALM2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("VICE", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("VICE", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("VICE", false),			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("VICE", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("VICE", true),				// Can this unit squash infantry?
		Read_Can_Harvest("VICE", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("VICE", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("VICE", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("VICE", false),			// Is there an associated firing animation?
		Read_Lock_Turret("VICE", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("VICE", false),			// Does it lay tracks while moving?
		Read_Is_Huge("VICE", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("VICE", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("VICE", true),				// Does the unit have a constant animation?
	Read_Ammo("VICE", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("VICE", 150),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("VICE", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("VICE", 800),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("VICE", 1),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("VICE", 20),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("VICE", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_GOOD | HOUSEF_BAD | HOUSEF_NEUTRAL | HOUSEF_JP, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("VICE", WEAPON_CHEMSPRAY),Read_Secondary_Weapon("VICE", WEAPON_NONE),
	Read_Armor("VICE", ARMOR_WOOD),								// ARMOR:		Armor type
	Read_Unit_Speed("VICE", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("VICE", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("VICE", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Flame tank
static UnitTypeClass const UnitFTank(
	UNIT_FTANK,
	TXT_FTANK,								// NAME:			Text name of this unit type.
	TXT_FTANK_STRING,
	"FTNK",									// NAME:			Text name of this unit type.
	ANIM_NAPALM3,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("FTNK", 4),							// Build level.
	STRUCT_RADAR,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("FTNK", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("FTNK", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("FTNK", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("FTNK", true),				// Can this unit squash infantry?
		Read_Can_Harvest("FTNK", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("FTNK", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("FTNK", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("FTNK", false),			// Is there an associated firing animation?
		Read_Lock_Turret("FTNK", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("FTNK", true),				// Does it lay tracks while moving?
		Read_Is_Huge("FTNK", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("FTNK", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("FTNK", false),			// Does the unit have a constant animation?
	Read_Ammo("FTNK", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("FTNK", 300),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("FTNK", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("FTNK", 800),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("FTNK", 9),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("FTNK", 66),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("FTNK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("FTNK", WEAPON_FLAME_TONGUE),Read_Secondary_Weapon("FTNK", WEAPON_NONE),
	Read_Armor("FTNK", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("FTNK", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("FTNK", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("FTNK", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Stealth tank
static UnitTypeClass const UnitSTank(
	UNIT_STANK,
	TXT_STANK,								// NAME:			Text name of this unit type.
	TXT_STANK_STRING,
	"STNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("STNK", 5),								// Build level.
	STRUCT_RADAR,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("STNK", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("STNK", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("STNK", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("STNK", true),				// Can this unit squash infantry?
		Read_Can_Harvest("STNK", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("STNK", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("STNK", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("STNK", false),			// Is there an associated firing animation?
		Read_Lock_Turret("STNK", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("STNK", true),				// Does it lay tracks while moving?
		Read_Is_Huge("STNK", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("STNK", true),				// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("STNK", false),			// Does the unit have a constant animation?
	Read_Ammo("STNK", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("STNK", 110),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("STNK", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("STNK", 900),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("STNK", 12),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("STNK", 81),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("STNK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("STNK", WEAPON_DRAGON),Read_Secondary_Weapon("STNK", WEAPON_NONE),
	Read_Armor("STNK", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("STNK", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("STNK", MPH_MEDIUM_FAST),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("STNK", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Light tank
static UnitTypeClass const UnitLTank(
	UNIT_LTANK,
	TXT_LTANK,								// NAME:			Text name of this unit type.
	TXT_LTANK_STRING,
	"LTNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("LTNK", 3),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("LTNK", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("LTNK", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("LTNK", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("LTNK", true),				// Can this unit squash infantry?
		Read_Can_Harvest("LTNK", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("LTNK", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("LTNK", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("LTNK", false),			// Is there an associated firing animation?
		Read_Lock_Turret("LTNK", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("LTNK", true),				// Does it lay tracks while moving?
		Read_Is_Huge("LTNK", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("LTNK", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("LTNK", false),			// Does the unit have a constant animation?
	Read_Ammo("LTNK", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("LTNK", 300),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("LTNK", 3),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("LTNK", 600),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("LTNK", 5),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("LTNK", 56),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("LTNK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("LTNK", WEAPON_75MM),Read_Secondary_Weapon("LTNK", WEAPON_NONE),
	Read_Armor("LTNK", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("LTNK", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("LTNK", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("LTNK", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Medium tank
static UnitTypeClass const UnitMTank(
	UNIT_MTANK,
	TXT_MTANK,								// NAME:			Text name of this unit type.
	TXT_MTANK_STRING,
	"MTNK",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("MTNK", 3),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("MTNK", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("MTNK", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("MTNK", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("MTNK", true),				// Can this unit squash infantry?
		Read_Can_Harvest("MTNK", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("MTNK", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("MTNK", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("MTNK", false),			// Is there an associated firing animation?
		Read_Lock_Turret("MTNK", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("MTNK", true),				// Does it lay tracks while moving?
		Read_Is_Huge("MTNK", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("MTNK", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("MTNK", false),			// Does the unit have a constant animation?
	Read_Ammo("MTNK", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("MTNK", 400),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("MTNK", 3),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MTNK", 800),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("MTNK", 7),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("MTNK", 62),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MTNK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("MTNK", WEAPON_105MM),Read_Secondary_Weapon("MTNK", WEAPON_NONE),
	Read_Armor("MTNK", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("MTNK", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("MTNK", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("MTNK", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mastadon tank
static UnitTypeClass const UnitHTank(
	UNIT_HTANK,
	TXT_HTANK,								// NAME:			Text name of this unit type.
	TXT_HTANK_STRING,
	"HTNK",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("HTNK", 5),								// Build level.
	STRUCT_REPAIR,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("HTNK", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("HTNK", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("HTNK", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("HTNK", true),				// Can this unit squash infantry?
		Read_Can_Harvest("HTNK", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("HTNK", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("HTNK", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("HTNK", false),			// Is there an associated firing animation?
		Read_Lock_Turret("HTNK", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("HTNK", true),				// Does it lay tracks while moving?
		Read_Is_Huge("HTNK", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("HTNK", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("HTNK", false),			// Does the unit have a constant animation?
	Read_Ammo("HTNK", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("HTNK", 600),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("HTNK", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("HTNK", 1500),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("HTNK", 13),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("HTNK", 80), 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HTNK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("HTNK", WEAPON_120MM),Read_Secondary_Weapon("HTNK", WEAPON_MAMMOTH_TUSK),
	Read_Armor("HTNK", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("HTNK", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("HTNK", MPH_MEDIUM_SLOW),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("HTNK", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mobile HQ
static UnitTypeClass const UnitMHQ(
	UNIT_MHQ,
	TXT_MHQ,									// NAME:			Text name of this unit type.
	TXT_MHQ_STRING,
	"MHQ",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("MHQ", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("MHQ", false),			// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		Read_Limited_Facings("MHQ", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("MHQ", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("MHQ", true),				// Can this unit squash infantry?
		Read_Can_Harvest("MHQ", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("MHQ", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("MHQ", true),				// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("MHQ", false),			// Is there an associated firing animation?
		Read_Lock_Turret("MHQ", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("MHQ", true),				// Does it lay tracks while moving?
		Read_Is_Huge("MHQ", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("MHQ", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("MHQ", false),			// Does the unit have a constant animation?
	Read_Ammo("MHQ", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("MHQ", 110),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("MHQ", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MHQ", 600),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("MHQ", 99),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("MHQ", 100),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MHQ", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("MHQ", WEAPON_NONE),Read_Secondary_Weapon("MHQ", WEAPON_NONE),
	Read_Armor("MHQ", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("MHQ", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("MHQ", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("MHQ", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Landing craft
static UnitTypeClass const UnitHover(
	UNIT_HOVER,
	TXT_HOVER,								// NAME:			Text name of this unit type.
	TXT_HOVER_STRING,
	"LST",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("LST", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("LST", false),			// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		Read_Limited_Facings("LST", false),			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("LST", true),				//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("LST", false),			// Can this unit squash infantry?
		Read_Can_Harvest("LST", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
//		true,			// Is selectable by player?
		false,			// Is selectable by player?
		false,			// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		true,				// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("LST", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("LST", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("LST", false),			// Is there an associated firing animation?
		Read_Lock_Turret("LST", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("LST", false),			// Does it lay tracks while moving?
		Read_Is_Huge("LST", true),				// Is this a gigundo-rotund-enormous unit?
		true,				// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("LST", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("LST", false),			// Does the unit have a constant animation?
	Read_Ammo("LST", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("LST", 400),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("LST", 3),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("LST", 300),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("LST", 99),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("LST", 40),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("LST", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("LST", WEAPON_NONE),Read_Secondary_Weapon("LST", WEAPON_NONE),
	Read_Armor("LST", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("LST", SPEED_HOVER),							// MOVE:			Locomotion type.
	Read_Speed("LST", MPH_MEDIUM_FAST),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("LST", 127),										// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Mobile sam launcher
static UnitTypeClass const UnitSAM(
	UNIT_MSAM,
	TXT_MSAM,								// NAME:			Text name of this unit type.
	TXT_MSAM_STRING,
	"MLRS",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("MLRS", 7),								// Build level.
	STRUCT_ATOWER,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("MLRS", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("MLRS", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("MLRS", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("MLRS", false),			// Can this unit squash infantry?
		Read_Can_Harvest("MLRS", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("MLRS", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("MLRS", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("MLRS", false),			// Is there an associated firing animation?
		Read_Lock_Turret("MLRS", true),				// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("MLRS", true),				// Does it lay tracks while moving?
		Read_Is_Huge("MLRS", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("MLRS", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("MLRS", false),			// Does the unit have a constant animation?
	Read_Ammo("MLRS", 2),											// AMMO:			Number of shots it has (default).
	Read_Strength("MLRS", 120),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("MLRS", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MLRS", 750),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("MLRS", 98),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("MLRS", 30),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MLRS", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP",
	Read_Primary_Weapon("MLRS", WEAPON_HONEST_JOHN),Read_Secondary_Weapon("MLRS", WEAPON_NONE),
	Read_Armor("MLRS", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("MLRS", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("MLRS", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("MLRS", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Artillery
static UnitTypeClass const UnitArty(
	UNIT_ARTY,
	TXT_ARTY,								// NAME:			Text name of this unit type.
	TXT_ARTY_STRING,
	"ARTY",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("ARTY", 6),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("ARTY", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("ARTY", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("ARTY", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("ARTY", false),			// Can this unit squash infantry?
		Read_Can_Harvest("ARTY", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("ARTY", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("ARTY", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("ARTY", false),			// Is there an associated firing animation?
		Read_Lock_Turret("ARTY", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("ARTY", true),				// Does it lay tracks while moving?
		Read_Is_Huge("ARTY", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("ARTY", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("ARTY", false),			// Does the unit have a constant animation?
	Read_Ammo("ARTY", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("ARTY", 75),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("ARTY", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("ARTY", 450),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("ARTY", 9),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("ARTY", 73),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("ARTY", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("ARTY", WEAPON_155MM),Read_Secondary_Weapon("ARTY", WEAPON_NONE),
	Read_Armor("ARTY", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("ARTY", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("ARTY", MPH_MEDIUM_SLOW),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("ARTY", 2),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Harvester
static UnitTypeClass const UnitHarvester(
	UNIT_HARVESTER,
	TXT_HARVESTER,							// NAME:			Text name of this unit type.
	TXT_HARVESTER_STRING,
	"HARV",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("HARV", 2),								// Build level.
	STRUCT_REFINERY,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("HARV", true),				// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		Read_Limited_Facings("HARV", false),			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("HARV", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("HARV", true),				// Can this unit squash infantry?
		Read_Can_Harvest("HARV", true),				// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("HARV", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("HARV", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("HARV", false),			// Is there an associated firing animation?
		Read_Lock_Turret("HARV", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("HARV", true),				// Does it lay tracks while moving?
		Read_Is_Huge("HARV", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("HARV", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("HARV", false),			// Does the unit have a constant animation?
	Read_Ammo("HARV", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("HARV", 600),										// STRENGTH:	Strength (in damage points).
//	300,										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("HARV", 2),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("HARV", 1400),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("HARV", 7),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("HARV", 85),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HARV", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("HARV", WEAPON_NONE),Read_Secondary_Weapon("HARV", WEAPON_NONE),
	Read_Armor("HARV", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("HARV", SPEED_WHEEL),							// MOVE:			Locomotion type.
	Read_Speed("HARV", MPH_MEDIUM_SLOW),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("HARV", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HARVEST						// ORDERS:		Default order to give new unit.
);

// Mobile construction vehicle
static UnitTypeClass const UnitMCV(
	UNIT_MCV,
	TXT_MCV,									// NAME:			Text name of this unit type.
	TXT_MCV_STRING,
	"MCV",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("MCV", 7),							// Build level.
	STRUCT_EYE,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("MCV", true),				// Can this be a goodie surprise from a crate?
		false,			// Is a leader type?
		Read_Limited_Facings("MCV", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("MCV", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("MCV", true),				// Can this unit squash infantry?
		Read_Can_Harvest("MCV", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("MCV", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("MCV", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("MCV", false),			// Is there an associated firing animation?
		Read_Lock_Turret("MCV", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("MCV", true),				// Does it lay tracks while moving?
		Read_Is_Huge("MCV", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("MCV", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("MCV", false),			// Does the unit have a constant animation?
	Read_Ammo("MCV", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("MCV", 600),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("MCV", 2),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MCV", 5000),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("MCV", 15),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("MCV", 86),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MCV", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("MCV", WEAPON_NONE),Read_Secondary_Weapon("MCV", WEAPON_NONE),
	Read_Armor("MCV", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("MCV", SPEED_WHEEL),							// MOVE:			Locomotion type.
	Read_Speed("MCV", MPH_MEDIUM_SLOW),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("MCV", 5),											// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Jeep (hummer)
static UnitTypeClass const UnitJeep(
	UNIT_JEEP,
	TXT_JEEP,								// NAME:			Text name of this unit type.
	TXT_JEEP_STRING,
	"JEEP",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("JEEP", 2),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("JEEP", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("JEEP", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("JEEP", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("JEEP", false),			// Can this unit squash infantry?
		Read_Can_Harvest("JEEP", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("JEEP", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("JEEP", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("JEEP", false),			// Is there an associated firing animation?
		Read_Lock_Turret("JEEP", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("JEEP", true),				// Does it lay tracks while moving?
		Read_Is_Huge("JEEP", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("JEEP", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("JEEP", false),			// Does the unit have a constant animation?
	Read_Ammo("JEEP", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("JEEP", 150),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("JEEP", 2),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("JEEP", 400),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("JEEP", 5),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("JEEP", 41),	 								// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("JEEP", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("JEEP", WEAPON_M60MG),Read_Secondary_Weapon("JEEP", WEAPON_NONE),
	Read_Armor("JEEP", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("JEEP", SPEED_WHEEL),							// MOVE:			Locomotion type.
	Read_Speed("JEEP", MPH_MEDIUM_FAST),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("JEEP", 10),										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Buggy
static UnitTypeClass const UnitBuggy(
	UNIT_BUGGY,
	TXT_DUNE_BUGGY,						// NAME:			Text name of this unit type.
	TXT_DUNE_BUGGY_STRING,
	"BGGY",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("BGGY", 4),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("BGGY", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("BGGY", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("BGGY", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("BGGY", false),			// Can this unit squash infantry?
		Read_Can_Harvest("BGGY", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("BGGY", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("BGGY", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("BGGY", false),			// Is there an associated firing animation?
		Read_Lock_Turret("BGGY", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("BGGY", false),			// Does it lay tracks while moving?
		Read_Is_Huge("BGGY", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("BGGY", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("BGGY", false),			// Does the unit have a constant animation?
	Read_Ammo("BGGY", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("BGGY", 140),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("BGGY", 2),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BGGY", 300),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("BGGY", 5),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("BGGY", 42), 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BGGY", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("BGGY", WEAPON_M60MG),Read_Secondary_Weapon("BGGY", WEAPON_NONE),
	Read_Armor("BGGY", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("BGGY", SPEED_WHEEL),							// MOVE:			Locomotion type.
	Read_Speed("BGGY", MPH_MEDIUM_FAST),						// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("BGGY", 10),										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Attack cycle
static UnitTypeClass const UnitBike(
	UNIT_BIKE,
	TXT_BIKE,								// NAME:			Text name of this unit type.
	TXT_BIKE_STRING,
	"BIKE",									// NAME:			Text name of this unit type.
	ANIM_FRAG1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("BIKE", 2),								// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("BIKE", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("BIKE", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("BIKE", false),			//	Is this a typical transport vehicle?
		false/*true*/,	// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("BIKE", false),			// Can this unit squash infantry?
		Read_Can_Harvest("BIKE", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("BIKE", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("BIKE", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("BIKE", false),			// Is there an associated firing animation?
		Read_Lock_Turret("BIKE", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("BIKE", false),			// Does it lay tracks while moving?
		Read_Is_Huge("BIKE", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("BIKE", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("BIKE", false),			// Does the unit have a constant animation?
	Read_Ammo("BIKE", -1),										// AMMO:			Number of shots it has (default).
#ifdef ADVANCED
	Read_Strength("BIKE", 90),										// STRENGTH:	Strength (in damage points).
#else
	Read_Strength("BIKE", 160),										// STRENGTH:	Strength (in damage points).
#endif
	Read_Sight_Range("BIKE", 2),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BIKE", 500),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("BIKE", 5),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("BIKE", 45), 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BIKE", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("BIKE", WEAPON_DRAGON),Read_Secondary_Weapon("BIKE", WEAPON_NONE),
	Read_Armor("BIKE", ARMOR_WOOD),								// ARMOR:		Armor type
	Read_Unit_Speed("BIKE", SPEED_WHEEL),							// MOVE:			Locomotion type.
	Read_Speed("BIKE", MPH_FAST),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("BIKE", 10),										// ROT:			Rate of turn (degrees per tick).
	0,							// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Rocket launcher
static UnitTypeClass const UnitMLRS(
	UNIT_MLRS,
	TXT_MLRS,								// NAME:			Text name of this unit type.
	TXT_MLRS_STRING,
	"MSAM",									// NAME:			Text name of this unit type.
	ANIM_ART_EXP1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("MSAM", 7),								// Build level.
	STRUCT_EYE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("MSAM", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("MSAM", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("MSAM", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("MSAM", false),			// Can this unit squash infantry?
		Read_Can_Harvest("MSAM", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("MSAM", true),				// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("MSAM", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("MSAM", false),			// Is there an associated firing animation?
		Read_Lock_Turret("MSAM", true),				// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("MSAM", true),				// Does it lay tracks while moving?
		Read_Is_Huge("MSAM", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("MSAM", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("MSAM", false),			// Does the unit have a constant animation?
	Read_Ammo("MSAM", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("MSAM", 100),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("MSAM", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MSAM", 800),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("MSAM", 11),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("MSAM", 72), 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MSAM", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("MSAM", WEAPON_MLRS),Read_Secondary_Weapon("MSAM", WEAPON_NONE),
	Read_Armor("MSAM", ARMOR_ALUMINUM),						// ARMOR:		Armor type
	Read_Unit_Speed("MSAM", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("MSAM", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("MSAM", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Armored personnel carrier
static UnitTypeClass const UnitAPC(
	UNIT_APC,
	TXT_APC,									// NAME:			Text name of this unit type.
	TXT_APC_STRING,
	"APC",									// NAME:			Text name of this unit type.
	ANIM_FRAG2,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("APC", 4),								// Build level.
	STRUCT_BARRACKS,			// Building prerequisite.
		Read_Can_Be_Found_In_Crate("APC", true),				// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("APC", false),			// Only has eight facings?
		false,			// Always use the given name for the vehicle?
		Read_Is_Transport("APC", true),				//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("APC", true),				// Can this unit squash infantry?
		Read_Can_Harvest("APC", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		true,				// Can it be repaired in a repair facility?
		Read_Is_Buildable("APC", true),				// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("APC", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("APC", false),			// Is there an associated firing animation?
		Read_Lock_Turret("APC", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("APC", true),				// Does it lay tracks while moving?
		Read_Is_Huge("APC", false),			// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("APC", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("APC", false),			// Does the unit have a constant animation?
	Read_Ammo("APC", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("APC", 200),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("APC", 4),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("APC", 700),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("APC", 5),											// SCENARIO:	Starting availability scenario.
	80, Read_Reward("APC", 15),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("APC", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_BAD | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("APC", WEAPON_M60MG),Read_Secondary_Weapon("APC", WEAPON_NONE),
	Read_Armor("APC", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("APC", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("APC", MPH_MEDIUM_FASTER),					// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("APC", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_HUNT							// ORDERS:		Default order to give new unit.
);

// Gunboat
static UnitTypeClass const UnitGunBoat(
	UNIT_GUNBOAT,
	TXT_GUNBOAT,							// NAME:			Text name of this unit type.
	TXT_GUNBOAT_STRING,
	"BOAT",									// NAME:			Text name of this unit type.
	ANIM_FBALL1,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("BOAT", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("BOAT", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("BOAT", false),			// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("BOAT", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("BOAT", false),			// Can this unit squash infantry?
		Read_Can_Harvest("BOAT", false),			// Does this unit harvest Tiberium?
		false,			// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		true,				// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		true,				// Is it equipped with a combat turret?
		true,				// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("BOAT", false),			// Can the player construct or order this unit?
		true,				// Is there a crew inside?
		Read_Radar_Equipped("BOAT", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("BOAT", false),			// Is there an associated firing animation?
		Read_Lock_Turret("BOAT", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("BOAT", false),			// Does it lay tracks while moving?
		Read_Is_Huge("BOAT", true),				// Is this a gigundo-rotund-enormous unit?
		true,				// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("BOAT", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("BOAT", false),			// Does the unit have a constant animation?
	Read_Ammo("BOAT", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("BOAT", 700),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("BOAT", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BOAT", 300),										// COST:			Cost to build (Credits).
	Read_Scenario_Level("BOAT", 99),										// SCENARIO:	Starting availability scenario.
	80, Read_Reward("BOAT", 40),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BOAT", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("BOAT", WEAPON_TOMAHAWK),Read_Secondary_Weapon("BOAT", WEAPON_NONE),
	Read_Armor("BOAT", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("BOAT", SPEED_FLOAT),							// MOVE:			Locomotion type.
	Read_Speed("BOAT", MPH_SLOW),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("BOAT", 1),											// ROT:			Rate of turn (degrees per tick).
	14,										// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Triceratops
static UnitTypeClass const UnitTric(
	UNIT_TRIC,
	TXT_TRIC,								// NAME:			Text name of this unit type.
	TXT_TRIC_STRING,
	"TRIC",									// NAME:			Text name of this unit type.
	ANIM_TRIC_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("TRIC", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("TRIC", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("TRIC", true),				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("TRIC", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("TRIC", true),				// Can this unit squash infantry?
		Read_Can_Harvest("TRIC", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("TRIC", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("TRIC", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("TRIC", true),				// Is there an associated firing animation?
		Read_Lock_Turret("TRIC", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("TRIC", false),			// Does it lay tracks while moving?
		Read_Is_Huge("TRIC", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("TRIC", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("TRIC", false),			// Does the unit have a constant animation?
	Read_Ammo("TRIC", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("TRIC", 700),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("TRIC", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("TRIC", 0),											// COST:			Cost to build (Credits).
	Read_Scenario_Level("TRIC", 99),										// SCENARIO:	Starting availability scenario.
	50, Read_Reward("TRIC", 50),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("TRIC", HOUSEF_JP, "JP"),								// OWNABLE:		Ownable by house (bit field).
	"JP",
	Read_Primary_Weapon("TRIC", WEAPON_STEG),Read_Secondary_Weapon("TRIC", WEAPON_NONE),
	Read_Armor("TRIC", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("TRIC", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("TRIC", MPH_SLOW),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("TRIC", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Tyrannosaurus Rex
static UnitTypeClass const UnitTrex(
	UNIT_TREX,
	TXT_TREX,								// NAME:			Text name of this unit type.
	TXT_TREX_STRING,
	"TREX",									// NAME:			Text name of this unit type.
	ANIM_TREX_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("TREX", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("TREX", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("TREX", true),				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("TREX", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("TREX", true),				// Can this unit squash infantry?
		Read_Can_Harvest("TREX", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("TREX", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("TREX", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("TREX", true),				// Is there an associated firing animation?
		Read_Lock_Turret("TREX", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("TREX", false),			// Does it lay tracks while moving?
		Read_Is_Huge("TREX", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("TREX", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("TREX", false),			// Does the unit have a constant animation?
	Read_Ammo("TREX", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("TREX", 750),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("TREX", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("TREX", 0),											// COST:			Cost to build (Credits).
	Read_Scenario_Level("TREX", 99),										// SCENARIO:	Starting availability scenario.
	50, Read_Reward("TREX", 50),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("TREX", HOUSEF_JP, "JP"),								// OWNABLE:		Ownable by house (bit field).
	"JP",
	Read_Primary_Weapon("TREX", WEAPON_TREX),Read_Secondary_Weapon("TREX", WEAPON_NONE),
	Read_Armor("TREX", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("TREX", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("TREX", MPH_MEDIUM),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("TREX", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Velociraptor
static UnitTypeClass const UnitRapt(
	UNIT_RAPT,
	TXT_RAPT,								// NAME:			Text name of this unit type.
	TXT_RAPT_STRING,
	"RAPT",									// NAME:			Text name of this unit type.
	ANIM_RAPT_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("RAPT", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("RAPT", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("RAPT", true),				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("RAPT", false),			//	Is this a typical transport vehicle?
		false/*true*/,	// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("RAPT", false),			// Can this unit squash infantry?
		Read_Can_Harvest("RAPT", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("RAPT", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("RAPT", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("RAPT", true),				// Is there an associated firing animation?
		Read_Lock_Turret("RAPT", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("RAPT", false),			// Does it lay tracks while moving?
		Read_Is_Huge("RAPT", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("RAPT", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("RAPT", false),			// Does the unit have a constant animation?
	Read_Ammo("RAPT", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("RAPT", 180),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("RAPT", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("RAPT", 0),											// COST:			Cost to build (Credits).
	Read_Scenario_Level("RAPT", 99),										// SCENARIO:	Starting availability scenario.
	50, Read_Reward("RAPT", 50),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("RAPT", HOUSEF_JP, "JP"),								// OWNABLE:		Ownable by house (bit field).
	"JP",
	Read_Primary_Weapon("RAPT", WEAPON_TREX),Read_Secondary_Weapon("RAPT", WEAPON_NONE),
	Read_Armor("RAPT", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("RAPT", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("RAPT", MPH_FAST),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("RAPT", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

// Stegosaurus
static UnitTypeClass const UnitSteg(
	UNIT_STEG,
	TXT_STEG,								// NAME:			Text name of this unit type.
	TXT_STEG_STRING,
	"STEG",									// NAME:			Text name of this unit type.
	ANIM_STEG_DIE,							// EXPLOSION:	Type of explosion when destroyed.
	Read_Build_Level("STEG", 99),							// Build level.
	STRUCT_NONE,				// Building prerequisite.
		Read_Can_Be_Found_In_Crate("STEG", false),			// Can this be a goodie surprise from a crate?
		true,				// Is a leader type?
		Read_Limited_Facings("STEG", true),				// Only has eight facings?
		true,				// Always use the given name for the vehicle?
		Read_Is_Transport("STEG", false),			//	Is this a typical transport vehicle?
		false,			// Can it be crushed by a heavy vehicle?
		Read_Can_Crush_Infantry("STEG", true),				// Can this unit squash infantry?
		Read_Can_Harvest("STEG", false),			// Does this unit harvest Tiberium?
		true,				// Is invisible to radar?
		true,				// Is selectable by player?
		true,				// Can it be a target for attack or move?
		false,			// Is it insignificant (won't be announced)?
		false,			// Is it immune to normal combat damage?
		false,			// Is it equipped with a combat turret?
		false,			// Fires multiple shots in quick succession?
		false,			// Can it be repaired in a repair facility?
		Read_Is_Buildable("STEG", false),			// Can the player construct or order this unit?
		false,			// Is there a crew inside?
		Read_Radar_Equipped("STEG", false),			// Does it have a rotating radar dish?
		Read_Has_Fire_Animation("STEG", true),				// Is there an associated firing animation?
		Read_Lock_Turret("STEG", false),			// Must the turret be in a locked down position while moving?
		Read_Has_Tracks("STEG", false),			// Does it lay tracks while moving?
		Read_Is_Huge("STEG", true),				// Is this a gigundo-rotund-enormous unit?
		false,			// Is the unit's art as "chunky" cardinal facing only?
		Read_Can_Cloak("STEG", false),			// Is the unit capable of cloaking?
		Read_Has_Constant_Animation("STEG", false),			// Does the unit have a constant animation?
	Read_Ammo("STEG", -1),										// AMMO:			Number of shots it has (default).
	Read_Strength("STEG", 600),										// STRENGTH:	Strength (in damage points).
	Read_Sight_Range("STEG", 5),											// SIGHTRANGE:	Range of sighting.
	Read_Cost("STEG", 0),											// COST:			Cost to build (Credits).
	Read_Scenario_Level("STEG", 99),										// SCENARIO:	Starting availability scenario.
	50, Read_Reward("STEG", 50),									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("STEG", HOUSEF_JP, "JP"),								// OWNABLE:		Ownable by house (bit field).
	"JP",
	Read_Primary_Weapon("STEG", WEAPON_STEG),Read_Secondary_Weapon("STEG", WEAPON_NONE),
	Read_Armor("STEG", ARMOR_STEEL),							// ARMOR:		Armor type
	Read_Unit_Speed("STEG", SPEED_TRACK),							// MOVE:			Locomotion type.
	Read_Speed("STEG", MPH_SLOW),								// SPEED:		Miles per hour.
	Read_Rate_Of_Turn("STEG", 5),											// ROT:			Rate of turn (degrees per tick).
	0,											// Turret center offset along body centerline.
	MISSION_GUARD							// ORDERS:		Default order to give new unit.
);

/*
**	This is the array of pointers to the static data associated with each
**	vehicle type.
*/
UnitTypeClass const * const UnitTypeClass::Pointers[UNIT_COUNT] = {
	&UnitHTank,			//	UNIT_HTANK
	&UnitMTank,			//	UNIT_MTANK
	&UnitLTank,			//	UNIT_LTANK
	&UnitSTank,			//	UNIT_STANK
	&UnitFTank,			//	UNIT_FTANK
	&UnitVisceroid,	// UNIT_VICE
	&UnitAPC,			//	UNIT_APC
	&UnitMLRS,			//	UNIT_MLRS
	&UnitJeep,			//	UNIT_JEEP
	&UnitBuggy,			//	UNIT_BUGGY
	&UnitHarvester,	//	UNIT_HARVESTER
	&UnitArty,			//	UNIT_ARTY
	&UnitSAM,			//	UNIT_MSAM
	&UnitHover,			//	UNIT_HOVER
	&UnitMHQ,			//	UNIT_MHQ
	&UnitGunBoat,		//	UNIT_GUNBOAT
	&UnitMCV,			// UNIT_MCV
	&UnitBike,			// UNIT_BIKE
	&UnitTric,			// UNIT_TRIC
	&UnitTrex,			// UNIT_TREX
	&UnitRapt,			// UNIT_RAPT
	&UnitSteg,			// UNIT_STEG
};


/***********************************************************************************************
 * UnitTypeClass::UnitTypeClass -- Constructor for unit types.                                 *
 *                                                                                             *
 *    This is the constructor for the unit types. It is used to initialize the unit type class *
 *    structure. The unit type class is used to control the behavior of the various types      *
 *    of units in the game. This constructor is called for every unique unit type as it        *
 *    exists in the array of unit types.                                                       *
 *                                                                                             *
 * INPUT:   bla bla bla... see below                                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/20/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitTypeClass::UnitTypeClass(UnitType type,
				int name,
				const char* friendlyName,
				char const *ininame,
				AnimType exp,
				unsigned char level,
				StructType pre,
				bool is_goodie,
				bool is_leader,
				bool is_eight,
				bool is_nominal,
				bool is_transporter,
				bool is_crushable,
				bool is_crusher,
				bool is_harvest,
				bool is_stealthy,
				bool is_selectable,
				bool is_legal_target,
				bool is_insignificant,
				bool is_immune,
				bool is_turret_equipped,
				bool is_twoshooter,
				bool is_repairable,
				bool is_buildable,
				bool is_crew,
				bool is_radar_equipped,
				bool is_fire_anim,
				bool is_lock_turret,
				bool is_tracked,
				bool is_gigundo,
				bool is_chunky,
				bool is_cloakable,
				bool is_animating,
				int ammo,
				unsigned short strength,
				int sightrange,
				int cost,
				int scenario,
				int risk,
				int reward,
				int ownable,
				const char* houseListCsv,
				WeaponType primary, WeaponType secondary,
				ArmorType armor,
				SpeedType speed,
				MPHType maxSpeed,
				unsigned rot,
				int toffset,
				MissionType order) :
					TechnoTypeClass(name,
										friendlyName,
										ininame,
										level,
										pre,
										is_leader,
										false,
										is_nominal,
										is_transporter,
										Read_Is_Flammable(ininame, false),
										is_crushable,
										is_stealthy,
										is_selectable,
										is_legal_target,
										is_insignificant,
										is_immune,
										false,
										is_twoshooter,
										is_turret_equipped,
										is_repairable,
										is_buildable,
										is_crew,
										ammo,
										strength,
										maxSpeed,
										sightrange,
										cost,
										scenario,
										risk,
										reward,
										ownable,
										houseListCsv,
										primary,secondary,
										armor)
{
	Explosion = exp;
	IsChunkyShape = is_chunky;
	Mission = order;
	TurretOffset = toffset;
	Type = type;

	IsCrateGoodie = Read_Can_Be_Found_In_Crate(IniName, is_goodie);
	IsPieceOfEight = Read_Limited_Facings(IniName, is_eight);
	IsCrusher = Read_Can_Crush_Infantry(IniName, is_crusher);
	IsToHarvest = Read_Can_Harvest(IniName, is_harvest);
	IsRadarEquipped = Read_Radar_Equipped(IniName, is_radar_equipped);
	IsFireAnim = Read_Has_Fire_Animation(IniName, is_fire_anim);
	IsLockTurret = Read_Lock_Turret(IniName, is_lock_turret);
	IsTracked = Read_Has_Tracks(IniName, is_tracked);
	IsGigundo = Read_Is_Huge(IniName, is_gigundo);
	IsCloakable = Read_Can_Cloak(IniName, is_cloakable);
	IsAnimating = Read_Has_Constant_Animation(IniName, is_animating); 
	Speed = Read_Unit_Speed(IniName, speed);
	ROT = Read_Rate_Of_Turn(IniName, rot);
}


/***********************************************************************************************
 * UnitTypeClass::Occupy_List -- Returns with unit occupation list.                            *
 *                                                                                             *
 *    This routine returns with an occupation list for the unit type.                          *
 *    The unit occupation list is used for placing units.                                      *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the unit occupation list.                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * UnitTypeClass::Occupy_List(bool ) const
{
	static short const _simple[] = {0, REFRESH_EOL};
	static short const _gun[] = {0, -1, 1, REFRESH_EOL};

	if (Type == UNIT_GUNBOAT) {
		return(&_gun[0]);
	}
	return(&_simple[0]);
}


/***********************************************************************************************
 * UnitTypeClass::From_Name -- Fetch class pointer from specified name.                        *
 *                                                                                             *
 *    This routine converts an ASCII representation of a unit class and                        *
 *    converts it into a real unit class number.                                               *
 *                                                                                             *
 * INPUT:   name  -- ASCII name representing a unit class.                                     *
 *                                                                                             *
 * OUTPUT:  Returns with the actual unit class number that the string                          *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
UnitType UnitTypeClass::From_Name(char const *name)
{
	if (name) {
		for (UnitType classid = UNIT_FIRST; classid < UNIT_COUNT; classid++) {
			if (stricmp(Pointers[classid]->IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(UNIT_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * UnitTypeClass::Display -- Displays a generic unit shape.                                    *
 *                                                                                             *
 *    This routine displays a generic representation of a unit of this                         *
 *    type. Typically, it is used when adding objects to the game map.                         *
 *                                                                                             *
 * INPUT:   x,y   -- Coordinate to render the unit shape.                                      *
 *                                                                                             *
 *          window-- Window to render within.                                                  *
 *                                                                                             *
 *          house -- House to render the unit colors.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/14/1994 JLB : Created.                                                                 *
 *   11/08/1994 JLB : Handles chunky type vehicles now.                                        *
 *=============================================================================================*/
void UnitTypeClass::Display(int x, int y, WindowNumberType window, HousesType house) const
{
	int shape = 0;
	void const * ptr = Get_Cameo_Data();
	if (!ptr) {
		ptr = Get_Image_Data();
		shape = IsChunkyShape ? 0 : 5;
	}
	CC_Draw_Shape(ptr, shape, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, HouseTypeClass::As_Reference(house).RemapTable);
}


/***********************************************************************************************
 * UnitTypeClass::Prep_For_Add -- Prepares scenario editor to add unit.                        *
 *                                                                                             *
 *    This routine is used to prepare the generic object adder dialog                          *
 *    box so that it will be able to add a unit object.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/04/1994 JLB : Uses map editing interface functions.                                    *
 *=============================================================================================*/
void UnitTypeClass::Prep_For_Add(void)
{
	for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data() != NULL) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * UnitTypeClass::One_Time -- Performs one time processing for unit type class objects.        *
 *                                                                                             *
 *    This routine is used to perform the action necessary only once for the unit type class.  *
 *    It loads unit shapes and brain files.   This routine should only be called once.         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::One_Time(void)
{
	for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char	buffer[_MAX_FNAME];
		UnitTypeClass const & uclass = As_Reference(index);
		CCFileClass	file;
		int			largest;			// Largest dimension of shape (so far).

		void const 	*ptr = NULL;		// Shape pointer and set pointer.

		largest = 0;
		if (uclass.IsBuildable) {

			/*
			**	Fetch the supporting data files for the unit.
			*/
			if ( Get_Resolution_Factor() ) {
				sprintf(buffer, "%sICNH", uclass.IniName);
			} else {
				sprintf(buffer, "%sICON", uclass.IniName);
			}
			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			((void const *&)uclass.CameoData) = MixFileClass::Retrieve(fullname);
		}

		/*
		**	Fetch a pointer to the unit's shape data.
		*/
		// Assume funpark mode might be required. ST - 10/14/2019 11:53AM
		//if (!uclass.IsPieceOfEight || (Special.IsJurassic && AreThingiesEnabled) ) {
			_makepath(fullname, NULL, NULL, uclass.IniName, ".SHP");
			ptr = MixFileClass::Retrieve(fullname);
		//} else {
		//	ptr = NULL;
		//}

		((void const *&)uclass.ImageData) = ptr;
		if (ptr) {

			if (index == UNIT_MLRS || index == UNIT_MSAM) {
				largest = 26;
			} else {
				largest = MAX(largest, (int)Get_Build_Frame_Width(ptr));
				largest = MAX(largest, (int)Get_Build_Frame_Height(ptr));
			}
		}

		((int &)uclass.MaxSize) = MAX(largest, 8);
	}

	/*
	**	Load the wake shapes in at this time.
	*/
	if (!WakeShapes) {
		WakeShapes = MixFileClass::Retrieve("WAKE.SHP");
	}
}



/***********************************************************************************************
 * UTC::Init -- fetches the sidebar icons for the unittypeclass objects                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    theater type                                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/26/96 4:07PM ST : Created                                                              *
 *=============================================================================================*/

void UnitTypeClass::Init(TheaterType theater)
{

	if (Get_Resolution_Factor()){

		if (theater != LastTheater){

			void const * cameo_ptr;
			char	fullname[_MAX_FNAME+_MAX_EXT];
			char	buffer[_MAX_FNAME];

			for (UnitType index = UNIT_FIRST; index < UNIT_COUNT; index++) {

				UnitTypeClass const & uclass = As_Reference(index);

				((void const *&)uclass.CameoData) = NULL;

				if (uclass.IsBuildable) {
					sprintf(buffer, "%sICNH", uclass.IniName);
					_makepath(fullname, NULL, NULL, buffer, Theaters[theater].Suffix);
					cameo_ptr = MixFileClass::Retrieve(fullname);
					if (cameo_ptr){
						((void const *&)uclass.CameoData) = cameo_ptr;
					}
				}
			}
		}
	}
}


/***********************************************************************************************
 * UnitTypeClass::Create_And_Place -- Creates and places a unit object onto the map.           *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place a unit object of this    *
 *    type onto the map.                                                                       *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the unit is to be placed into.                           *
 *                                                                                             *
 *          house    -- The house that the unit belongs to.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the unit created and placed successfully?                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool UnitTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	UnitClass * unit = new UnitClass(Type, house);
	if (unit) {
		return(unit->Unlimbo(Cell_Coord(cell), Random_Pick(DIR_N, DIR_MAX)));
	}
	return(false);
}


/***********************************************************************************************
 * UnitTypeClass::Create_One_Of -- Creates a unit in limbo.                                    *
 *                                                                                             *
 *    This function creates a unit of this type and keeps it in limbo. A pointer to the        *
 *    created unit object is returned. It is presumed that this object will later be           *
 *    unlimboed at the correct time and place.                                                 *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is to own the unit.                             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the created unit object. If the unit object              *
 *          could not be created, then NULL is returned.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * UnitTypeClass::Create_One_Of(HouseClass * house) const
{
	return(new UnitClass(Type, house->Class->House));
}


/***********************************************************************************************
 * UnitTypeClass::Who_Can_Build_Me -- Determines which factory can build this unit type.       *
 *                                                                                             *
 *    Use this routine to examine the buildings on the map in order to determine which one     *
 *    can build the unit type.                                                                 *
 *                                                                                             *
 * INPUT:   intheory -- If this parameter is true, then no examination of whether the factory  *
 *                      is currently busy is performed. It just checks to see if the unit      *
 *                      could be produced "in theory".                                         *
 *                                                                                             *
 *          legal    -- Should building prerequisite legality checks be performed as well?     *
 *                      For building placements, this is usually false. For sidebar button     *
 *                      adding, this is usually true.                                          *
 *                                                                                             *
 *          house    -- The owner of the unit to be produced.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to the factory that can produce the unit. If no suitable      *
 *          factory could be found then NULL is returned.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingClass * UnitTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	BuildingClass * anybuilding = NULL;

	for (int index = 0; index < Buildings.Count(); index++) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building &&
				!building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Class->ToBuild == RTTI_UNITTYPE &&
				building->Mission != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Ownable) &&
				(!legal || building->House->Can_Build(Type, building->ActLike)) &&
				(intheory || !building->In_Radio_Contact())) {

			if (building->IsLeader) return(building);
			anybuilding = building;
		}
	}
	return(anybuilding);
}


/***********************************************************************************************
 * UnitTypeClass::As_Reference -- Fetches a reference to the unit type class specified.        *
 *                                                                                             *
 *    Use this routine to return a reference to the UnitTypeClass object as indicated by       *
 *    the unit type number speicified.                                                         *
 *                                                                                             *
 * INPUT:   type  -- The unit type number to convert into a UnitTypeClass object reference.    *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the unit type class object specified.                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
UnitTypeClass const & UnitTypeClass::As_Reference(UnitType type)
{
	return(*Pointers[type]);
}


/***********************************************************************************************
 * UnitTypeClass::Dimensions -- Determines the unit's pixel dimensions.                        *
 *                                                                                             *
 *    This routine will fill in the width and height for this unit type. This width and        *
 *    height are used to render the selection rectangle and the positioning of the health      *
 *    bargraph.                                                                                *
 *                                                                                             *
 * INPUT:   width    -- Reference to the width of the unit (to be filled in).                  *
 *                                                                                             *
 *          height   -- Reference to the height of the unit (to be filled in).                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void UnitTypeClass::Dimensions(int &width, int &height) const
{
	if (Type == UNIT_GUNBOAT) {
		width = 46;
		height = 18;
	} else {
		width = MaxSize-(MaxSize/4);
		height = MaxSize-(MaxSize/4);
	}
}

/***********************************************************************************************
 * UnitTypeClass::Repair_Cost -- Determines cost per "step" of repair.                         *
 *                                                                                             *
 *    Use this routine to determine how much it will cost to repair the unit one               *
 *    step. A step is defined as the number of hit points returned from the Repair_Step()      *
 *    function.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit cost to repair this unit one step.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Repair_Cost(void) const
{
	auto repairFactor = Read_Double_Game_Rule(UNIT_REPAIR_FACTOR_RULE, 1.02f, 0.01f);
	auto repairPercent = nearbyint(repairFactor * 100);

	return(
		Fixed_To_Cardinal(
			Cost / (MaxStrength/ Read_Unit_Repair_Step_Rule(4)),
			repairPercent
		)
	);
}


/***********************************************************************************************
 * UnitTypeClass::Repair_Step -- Determines the repair step rate.                              *
 *                                                                                             *
 *    This routine will determine how many strength points get healed for each "step". The     *
 *    cost to repair one step is determine from the Repair_Cost() function.                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of health points repaired for each "step".                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/03/1995 BWG : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Repair_Step(void) const
{
	return(Read_Unit_Repair_Step_Rule(4));
}


/***********************************************************************************************
 * UnitTypeClass::Max_Pips -- Fetches the maximum pips allowed for this unit.                  *
 *                                                                                             *
 *    This routine will determine the number of pips (maximum) allowed for this unit type.     *
 *    Typically, this is the number of passengers allowed, but for harvesters, it is the       *
 *    number of credits it holds divided by 100.                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the maximum number of pips allowed for this unit type.                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/26/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int UnitTypeClass::Max_Pips(void) const
{
	if (this->IsToHarvest) {
		return(
			Read_Harvester_Max_Credits_Rule() / 100
		);
	}

	if (IsTransporter) {
		return(Max_Passengers());
	}

	return(0);
}

