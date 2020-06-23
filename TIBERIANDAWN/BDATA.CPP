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

/* $Header:   F:\projects\c&c\vcs\code\bdata.cpv   2.17   16 Oct 1995 16:50:08   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : July 17, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BuildingTypeClass::As_Reference -- Fetches reference to the building type specified.      *
 *   BuildingTypeClass::Bib_And_Offset -- Determines the bib and appropriate cell offset.      *
 *   BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.   *
 *   BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map. *
 *   BuildingTypeClass::Create_One_Of -- Creates a building of this type.                      *
 *   BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.            *
 *   BuildingTypeClass::Display -- Renders a generic view of building.                         *
 *   BuildingTypeClass::Full_Name -- Fetches the full name text number.                        *
 *   BuildingTypeClass::Height -- Determins the height of the building in icons.               *
 *   BuildingTypeClass::Init -- Performs theater specific initialization.                      *
 *   BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.           *
 *   BuildingTypeClass::Legal_Placement -- Determines if building can be legally placed at pos.*
 *   BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                    *
 *   BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.               *
 *   BuildingTypeClass::One_Time -- Performs special one time action for buildings.            *
 *   BuildingTypeClass::Overlap_List -- Fetches the overlap list for the building.             *
 *   BuildingTypeClass::Prep_For_Add -- Prepares scenario editor for adding an object.         *
 *   BuildingTypeClass::Repair_Cost -- Determines cost per "step" of repair.                   *
 *   BuildingTypeClass::Repair_Step -- Determines the repair step rate.                        *
 *   BuildingTypeClass::Who_Can_Build_Me -- Determines which factory can create the building.  *
 *   BuildingTypeClass::Width -- Determines width of bulding in icons.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#define	MCW	MAP_CELL_W

#define XYCELL(x,y)	(y*MAP_CELL_W+x)
static short const ExitPyle[] = {
	XYCELL(1,2),
	XYCELL(2,2),
	XYCELL(0,2),
	XYCELL(-1,2),
	XYCELL(-1,-1),
	XYCELL(0,-1),
	XYCELL(1,-1),
	XYCELL(2,-1),
	XYCELL(2,-1),
	XYCELL(-1,0),
	XYCELL(2,0),
	XYCELL(2,1),
	XYCELL(-1,1),
	REFRESH_EOL
};
static short const ExitHand[] = {
	XYCELL(2,3),
	XYCELL(1,3),
	XYCELL(0,3),
	XYCELL(2,2),
	XYCELL(-1,3),
	XYCELL(-1,2),
	XYCELL(0,0),
	XYCELL(1,0),
	XYCELL(-1,0),
	XYCELL(2,0),
	XYCELL(2,1),
	XYCELL(-1,1),
	REFRESH_EOL
};
static short const ExitWeap[] = {
	XYCELL(-1,3),
	XYCELL(0,3),
	XYCELL(-1,2),
	XYCELL(1,3),
//	XYCELL(0,0),
//	XYCELL(1,0),
//	XYCELL(2,0),
//	XYCELL(-1,0),
//	XYCELL(3,0),
	XYCELL(-1,1),
	XYCELL(3,1),
	XYCELL(3,2),
	XYCELL(3,3),
	XYCELL(2,3),
	REFRESH_EOL
};
static short const ExitAirstrip[] = {
	XYCELL(-1,-1),
	XYCELL(-1,0),
	XYCELL(-1,1),
	XYCELL(-1,2),
	XYCELL(0,-1),
	XYCELL(0,2),
	XYCELL(1,-1),
	XYCELL(1,2),
	XYCELL(2,-1),
	XYCELL(2,2),
	XYCELL(3,-1),
	XYCELL(3,2),
	XYCELL(4,-1),
	XYCELL(4,0),
	XYCELL(4,1),
	XYCELL(4,2),
	REFRESH_EOL
};

static short const OListSAM[] = {-MCW, -(MCW-1), REFRESH_EOL};
static short const List32[] = {0, 1, 2, MCW, MCW+1, MCW+2, REFRESH_EOL};
static short const List22_0011[] = {MCW, MCW+1, REFRESH_EOL};
static short const List22_1100[] = {0, 1, REFRESH_EOL};
static short const ListFix[] = {1, MCW, MCW+1, MCW+2, MCW+MCW+1, REFRESH_EOL};
static short const StoreList[] = {0, 1, REFRESH_EOL};
static short const List2[] = {0, 1, MCW+1, MCW, REFRESH_EOL};
static short const List42[] = {0, 1, 2, 3, MCW, MCW+1, MCW+2, MCW+3, REFRESH_EOL};
static short const ListWestwood[] = {1, 2, 3, MCW+1, MCW+2, MCW+3, REFRESH_EOL};
static short const OListWestwood[] = {0, MCW, REFRESH_EOL};
static short const ComList[] = {0, MCW, MCW+1, REFRESH_EOL};
static short const List21[] = {0, 1, REFRESH_EOL};
static short const ListWeap[] = {(MCW*1), (MCW*1)+1, (MCW*1)+2, (MCW*2), (MCW*2)+1, (MCW*2)+2, REFRESH_EOL};
static short const List12[] = {MCW, REFRESH_EOL};
static short const ListHand[] = {MCW, MCW+1, MCW*2+1, REFRESH_EOL};
static short const ListTmpl[] = {MCW, MCW+1, MCW+2, MCW*2, MCW*2+1, MCW*2+2, REFRESH_EOL};
static short const List0011[] = {(MCW*1), (MCW*1)+1, REFRESH_EOL};
static short const List1101[] = {0, 1, (MCW*1)+1, REFRESH_EOL};
static short const List11[] = {0, 1, REFRESH_EOL};
static short const List1[] = {0, REFRESH_EOL};
static short const List1100[] = {0, 1, REFRESH_EOL};
static short const List0010[] = {MCW, REFRESH_EOL};
static short const List1000[] = {0, REFRESH_EOL};
static short const List0100[] = {1, REFRESH_EOL};
static short const List0111[] = {1, (MCW*1), (MCW*1)+1, REFRESH_EOL};
//static short const List1111[] = {0, 1, (MCW*1), (MCW*1)+1, REFRESH_EOL};
static short const List1011[] = {0, (MCW*1), (MCW*1)+1, REFRESH_EOL};
static short const List010111000[] = {1, (MCW*1), (MCW*1)+1, (MCW*1)+2, REFRESH_EOL};
static short const List101000111[] = {0, 2, (MCW*2), (MCW*2)+1, (MCW*2)+2, REFRESH_EOL};

static short const OListFix[] = {0, 2, MCW+MCW, MCW+MCW+2, REFRESH_EOL};
static short const OListWeap[] = {0, 1, 2, REFRESH_EOL};
static short const OComList[] = {1, REFRESH_EOL};
static short const OList12[] = {0, REFRESH_EOL};
static short const OListHand[] = {0, 1, MCW*2, MCW*1, REFRESH_EOL};
static short const OListTmpl[] = {0, 1, 2, REFRESH_EOL};


/***************************************************************************
*/
static BuildingTypeClass const ClassTemple(
	STRUCT_TEMPLE,
	TXT_TEMPLE,								// NAME:			Short name of the structure.
	"TMPL",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("TMPL", 7),								// Build level.
	Read_Prerequisite("TMPL", STRUCT_RADAR),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("TMPL", false),		// Can this building be captured?
		Read_Is_Flammable("TMPL", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("TMPL", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("TMPL", 1000),										// STRNTH:		Full strength of building.
	4,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("TMPL", 3000),										// COST:			Cost to purchase.
	Read_Scenario_Level("TMPL", 13),										// SCENARIO:	Starting availability scenario.
	0,20, 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("TMPL", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("TMPL", WEAPON_NONE),Read_Secondary_Weapon("TMPL", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("TMPL", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("TMPL", 0),											// POWER:		Power points generated.
	Read_Power_Input("TMPL", 150),										// DRAIN:		Power points required.
	BSIZE_33, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)ListTmpl,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListTmpl		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassEye(
	STRUCT_EYE,
	TXT_EYE,									// NAME:			Short name of the structure.
	"EYE",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("EYE", 7),								// Build level.
	Read_Prerequisite("EYE", STRUCT_RADAR),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("EYE", true),			// Can this building be captured?
		Read_Is_Flammable("EYE", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("EYE", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	(DirType)160,							// Starting idle frame to match construction.
	Read_Strength("EYE", 500),										// STRNTH:		Full strength of building.
	10,										// SIGHTRANGE:	Range of sighting.
	Read_Cost("EYE", 2800),										// COST:			Cost to purchase.
	Read_Scenario_Level("EYE", 13),										// SCENARIO:	Starting availability scenario.
	0,100,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("EYE", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("EYE", WEAPON_NONE),Read_Secondary_Weapon("EYE", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("EYE", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("EYE", 0),											// POWER:		Power points required.
	Read_Power_Input("EYE", 200),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)ComList,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OComList		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassWeapon(
	STRUCT_WEAP,
	TXT_WEAPON_FACTORY,					// NAME:			Short name of the structure.
	"WEAP",									// NAME:			Short name of the structure.
	XYP_COORD(10+(CELL_PIXEL_W/2), ((CELL_PIXEL_H*3)-(CELL_PIXEL_H/2))-21),			// Exit point for produced units.
	Read_Build_Level("WEAP", 2),								// Build level.
	Read_Prerequisite("WEAP", STRUCT_REFINERY),			// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,			// Is it a factory type building?
		Read_Is_Capturable("WEAP", true),			// Can this building be captured?
		Read_Is_Flammable("WEAP", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("WEAP", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_UNITTYPE,							// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
#ifdef ADVANCED
	Read_Strength("WEAP", 500),										// STRNTH:		Full strength of building.
#else
	Read_Strength("WEAP", 200),										// STRNTH:		Full strength of building.
#endif
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("WEAP", 2000),										// COST:			Cost to purchase.
	Read_Scenario_Level("WEAP", 5),											// SCENARIO:	Starting availability scenario.
	0,86,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("WEAP", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("WEAP", WEAPON_NONE),Read_Secondary_Weapon("WEAP", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("WEAP", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("WEAP", 0),											// POWER:		Power points required.
	Read_Power_Input("WEAP", 30),										// DRAIN:		Power points required.
	BSIZE_33, 								// SIZE:			Building size.
	(short const *)ExitWeap,		// Preferred exit cell list.
	(short const *)ListWeap,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListWeap		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassGTower(
	STRUCT_GTOWER,
	TXT_GUARD_TOWER,						// NAME:			Short name of the structure.
	"GTWR",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("GTWR", 2),								// Build level.
	Read_Prerequisite("GTWR", STRUCT_BARRACKS),			// Building prerequisite.
		true,			// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("GTWR", false),		// Can this building be captured?
		Read_Is_Flammable("GTWR", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("GTWR", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("GTWR", 200),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("GTWR", 500),										// COST:			Cost to purchase.
	Read_Scenario_Level("GTWR", 7),											// SCENARIO:	Starting availability scenario.
	100,25,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("GTWR", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("GTWR", WEAPON_CHAIN_GUN),Read_Secondary_Weapon("GTWR", WEAPON_NONE),
//	WEAPON_M60MG,WEAPON_NONE,
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("GTWR", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("GTWR", 00),										// POWER:		Power points required.
	Read_Power_Input("GTWR", 10),										// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassATower(
	STRUCT_ATOWER,
	TXT_AGUARD_TOWER,						// NAME:			Short name of the structure.
	"ATWR",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("ATWR", 4),								// Build level.
	Read_Prerequisite("ATWR", STRUCT_RADAR),				// Building prerequisite.
		true,			// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("ATWR", false),		// Can this building be captured?
		Read_Is_Flammable("ATWR", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		true,			// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("ATWR", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("ATWR", 300),										// STRNTH:		Full strength of building.
	4,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("ATWR", 1000),										// COST:			Cost to purchase.
	Read_Scenario_Level("ATWR", 13),										// SCENARIO:	Starting availability scenario.
	100,30,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("ATWR", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("ATWR", WEAPON_TOW_TWO),Read_Secondary_Weapon("ATWR", WEAPON_NONE),
//	WEAPON_TOMAHAWK,WEAPON_NONE,
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("ATWR", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("ATWR", 0),											// POWER:		Power points required.
	Read_Power_Input("ATWR", 20),										// DRAIN:		Power points required.
	BSIZE_12, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List12,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)OList12			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassObelisk(
	STRUCT_OBELISK,
	TXT_OBELISK,							// NAME:			Short name of the structure.
	"OBLI",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("OBLI", 4),								// Build level.
	Read_Prerequisite("OBLI", STRUCT_RADAR),				// Building prerequisite.
		true,			// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("OBLI", false),		// Can this building be captured?
		Read_Is_Flammable("OBLI", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("OBLI", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("OBLI", 200),										// STRNTH:		Full strength of building.
	5,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("OBLI", 1500),										// COST:			Cost to purchase.
	Read_Scenario_Level("OBLI", 11),										// SCENARIO:	Starting availability scenario.
	100,35,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("OBLI", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("OBLI", WEAPON_OBELISK_LASER),Read_Secondary_Weapon("OBLI", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("OBLI", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("OBLI", 0),											// POWER:		Power points required.
	Read_Power_Input("OBLI", 150),										// DRAIN:		Power points required.
	BSIZE_12, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List12,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)OList12			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassTurret(
	STRUCT_TURRET,
	TXT_TURRET,								// NAME:			Short name of the structure.
	"GUN",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("GUN", 2),								// Build level.
	Read_Prerequisite("GUN", STRUCT_BARRACKS),			// Building prerequisite.
		true,			// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("GUN", false),		// Can this building be captured?
		Read_Is_Flammable("GUN", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		true,			// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("GUN", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	(DirType)208,							// Starting idle frame to match construction.
	Read_Strength("GUN", 200),										// STRNTH:		Full strength of building.
	5,											// SIGHTRANGE:	Range of sighting.
#ifdef ADVANCED
	Read_Cost("GUN", 600),										// COST:			Cost to purchase.
#else
#ifdef PATCH
	Read_Cost("GUN", 600),
#else
	Read_Cost("GUN", 250),										// COST:			Cost to purchase.
#endif
#endif
	Read_Scenario_Level("GUN", 8),											// SCENARIO:	Starting availability scenario.
	300,26,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("GUN", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("GUN", WEAPON_TURRET_GUN),Read_Secondary_Weapon("GUN", WEAPON_NONE),
	ARMOR_STEEL,							// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("GUN", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("GUN", 0),											// POWER:		Power points required.
	Read_Power_Input("GUN", 20),										// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassConst(
	STRUCT_CONST,
	TXT_CONST_YARD,						// NAME:			Short name of the structure.
	"FACT",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("FACT", 99),							// Build level.
	Read_Prerequisite("FACT", STRUCT_NONE),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,			// Is it a factory type building?
		Read_Is_Capturable("FACT", true),			// Can this building be captured?
		Read_Is_Flammable("FACT", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("FACT", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_BUILDINGTYPE,					// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("FACT", 400),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("FACT", 5000),										// COST:			Cost to purchase.
	Read_Scenario_Level("FACT", 1),											// SCENARIO:	Starting availability scenario.
	0,70,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("FACT", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("FACT", WEAPON_NONE),Read_Secondary_Weapon("FACT", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("FACT", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("FACT", 30),										// POWER:		Power points required.
	Read_Power_Input("FACT", 15),										// DRAIN:		Power points required.
	BSIZE_32, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List32,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassRefinery(
	STRUCT_REFINERY,
	TXT_REFINERY,							// NAME:			Short name of the structure.
	"PROC",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("PROC", 1),								// Build level.
	Read_Prerequisite("PROC", STRUCT_POWER),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("PROC", true),			// Can this building be captured?
		Read_Is_Flammable("PROC", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("PROC", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("PROC", 450),										// STRNTH:		Full strength of building.
	4,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("PROC", 2000),										// COST:			Cost to purchase.
	Read_Scenario_Level("PROC", 2),											// SCENARIO:	Starting availability scenario.
	0,55,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("PROC", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("PROC", WEAPON_NONE),Read_Secondary_Weapon("PROC", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("PROC", 1000),										// CAPACITY:	Spice storage capacity.
	Read_Power_Output("PROC", 10),										// POWER:		Power points required.
	Read_Power_Input("PROC", 40),										// DRAIN:		Power points required.
	BSIZE_33, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List010111000,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)List101000111		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassStorage(
	STRUCT_STORAGE,
	TXT_STORAGE,							// NAME:			Short name of the structure.
	"SILO",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("SILO", 1),								// Build level.
	Read_Prerequisite("SILO", STRUCT_REFINERY),			// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("SILO", true),			// Can this building be captured?
		Read_Is_Flammable("SILO", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("SILO", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("SILO", 150),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("SILO", 150),										// COST:			Cost to purchase.
//	300,										// COST:			Cost to purchase.
	Read_Scenario_Level("SILO", 2),											// SCENARIO:	Starting availability scenario.
	0,16,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("SILO", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("SILO", WEAPON_NONE),Read_Secondary_Weapon("SILO", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("SILO", 1500),										// CAPACITY:	Spice storage capacity.
	Read_Power_Output("SILO", 0),											// POWER:		Power points required.
	Read_Power_Input("SILO", 10),										// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)StoreList,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassHelipad(
	STRUCT_HELIPAD,
	TXT_HELIPAD,							// NAME:			Short name of the structure.
	"HPAD",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("HPAD", 6),								// Build level.
	Read_Prerequisite("HPAD", STRUCT_BARRACKS),			// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,		// Is it a factory type building?
		Read_Is_Capturable("HPAD", true),			// Can this building be captured?
		Read_Is_Flammable("HPAD", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("HPAD", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_AIRCRAFTTYPE,					// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("HPAD", 400),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("HPAD", 1500),										// COST:			Cost to purchase.
	Read_Scenario_Level("HPAD", 10),										// SCENARIO:	Starting availability scenario.
	0,65,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HPAD", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("HPAD", WEAPON_NONE),Read_Secondary_Weapon("HPAD", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("HPAD", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("HPAD", 0),											// POWER:		Power points required.
	Read_Power_Input("HPAD", 10),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List2,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassCommand(
	STRUCT_RADAR,
	TXT_COMMAND,							// NAME:			Short name of the structure.
	"HQ",										// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("HQ", 2),								// Build level.
	Read_Prerequisite("HQ", STRUCT_REFINERY),			// Building prerequisite.
		true,			// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("HQ", true),			// Can this building be captured?
		Read_Is_Flammable("HQ", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("HQ", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	(DirType)160,							// Starting idle frame to match construction.
	Read_Strength("HQ", 500),										// STRNTH:		Full strength of building.
	10,										// SIGHTRANGE:	Range of sighting.
	Read_Cost("HQ", 1000),										// COST:			Cost to purchase.
	Read_Scenario_Level("HQ", 3),											// SCENARIO:	Starting availability scenario.
	0,20, 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HQ", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("HQ", WEAPON_NONE),Read_Secondary_Weapon("HQ", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("HQ", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("HQ", 0),											// POWER:		Power points required.
	Read_Power_Input("HQ", 40),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)ComList,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OComList		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassSAM(
	STRUCT_SAM,
	TXT_SAM,									// NAME:			Short name of the structure.
	"SAM",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("SAM", 6),								// Build level.
	Read_Prerequisite("SAM", STRUCT_BARRACKS),			// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("SAM", false),		// Can this building be captured?
		Read_Is_Flammable("SAM", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		true,			// Does it have a rotating turret?
		true,			// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("SAM", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("SAM", 200),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("SAM", 750),										// COST:			Cost to purchase.
	Read_Scenario_Level("SAM", 5),											// SCENARIO:	Starting availability scenario.
	300,40,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("SAM", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("SAM", WEAPON_NIKE),Read_Secondary_Weapon("SAM", WEAPON_NONE),
	ARMOR_STEEL,							// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("SAM", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("SAM", 0),											// POWER:		Power points required.
	Read_Power_Input("SAM", 20),										// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List21,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListSAM				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAirStrip(
	STRUCT_AIRSTRIP,
	TXT_AIRSTRIP,							// NAME:			Short name of the structure.
	"AFLD",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("AFLD", 2),								// Build level.
	Read_Prerequisite("AFLD", STRUCT_REFINERY),			// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,			// Is it a factory type building?
		Read_Is_Capturable("AFLD", true),			// Can this building be captured?
		Read_Is_Flammable("AFLD", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("AFLD", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_UNITTYPE,							// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("AFLD", 500),										// STRNTH:		Full strength of building.
	5,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("AFLD", 2000),										// COST:			Cost to purchase.
	Read_Scenario_Level("AFLD", 5),											// SCENARIO:	Starting availability scenario.
	300,86,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("AFLD", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("AFLD", WEAPON_NONE),Read_Secondary_Weapon("AFLD", WEAPON_NONE),
	ARMOR_STEEL,							// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("AFLD", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("AFLD", 0),											// POWER:		Power points required.
	Read_Power_Input("AFLD", 30),										// DRAIN:		Power points required.
	BSIZE_42, 								// SIZE:			Building size.
	ExitAirstrip,							// Preferred exit cell list.
	(short const *)List42,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassPower(
	STRUCT_POWER,
	TXT_POWER,								// NAME:			Short name of the structure.
	"NUKE",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("NUKE", 0),								// Build level.
	Read_Prerequisite("NUKE", STRUCT_NONE),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("NUKE", true),			// Can this building be captured?
		Read_Is_Flammable("NUKE", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("NUKE", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("NUKE", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("NUKE", 300),										// COST:			Cost to purchase.
	Read_Scenario_Level("NUKE", 1),											// SCENARIO:	Starting availability scenario.
	0,50,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("NUKE", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("NUKE", WEAPON_NONE),Read_Secondary_Weapon("NUKE", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("NUKE", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("NUKE", 100),										// POWER:		Power points required.
	Read_Power_Input("NUKE", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List0100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassAdvancedPower(
	STRUCT_ADVANCED_POWER,
	TXT_ADVANCED_POWER,					// NAME:			Short name of the structure.
	"NUK2",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("NUK2", 5),											// Build level.
	Read_Prerequisite("NUK2", STRUCT_POWER),							// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("NUK2", true),			// Can this building be captured?
		Read_Is_Flammable("NUK2", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("NUK2", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("NUK2", 300),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("NUK2", 700),										// COST:			Cost to purchase.
	Read_Scenario_Level("NUK2", 13),										// SCENARIO:	Starting availability scenario.
	0,75,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("NUK2", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("NUK2", WEAPON_NONE),Read_Secondary_Weapon("NUK2", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("NUK2", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("NUK2", 200),										// POWER:		Power points required.
	Read_Power_Input("NUK2", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List0100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassHospital(
	STRUCT_HOSPITAL,
	TXT_HOSPITAL,							// NAME:			Short name of the structure.
	"HOSP",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("HOSP", 99),							// Build level.
	Read_Prerequisite("HOSP", STRUCT_BARRACKS),			// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("HOSP", true),			// Can this building be captured?
		Read_Is_Flammable("HOSP", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("HOSP", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("HOSP", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("HOSP", 500),										// COST:			Cost to purchase.
	Read_Scenario_Level("HOSP", 99),										// SCENARIO:	Starting availability scenario.
	0,20, 									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HOSP", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("HOSP", WEAPON_NONE),Read_Secondary_Weapon("HOSP", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("HOSP", 100),										// CAPACITY:	Spice storage capacity.
	Read_Power_Output("HOSP", 0),											// POWER:		Power points required.
	Read_Power_Input("HOSP", 20),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List2,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL,			// OVERLAPLIST:List of overlap cell offset.
	true							// Is this building un-sellable?
);

static BuildingTypeClass const ClassBioLab(
	STRUCT_BIO_LAB,
	TXT_BIO_LAB,							// NAME:			Short name of the structure.
	"BIO",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("BIO", 99),							// Build level.
	Read_Prerequisite("BIO", STRUCT_HOSPITAL),			// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("BIO", false),		// Can this building be captured?
		Read_Is_Flammable("BIO", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("BIO", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("BIO", 300),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BIO", 500),										// COST:			Cost to purchase.
	Read_Scenario_Level("BIO", 99),										// SCENARIO:	Starting availability scenario.
	0,1,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BIO", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("BIO", WEAPON_NONE),Read_Secondary_Weapon("BIO", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("BIO", 100),										// CAPACITY:	Spice storage capacity.
	Read_Power_Output("BIO", 0),											// POWER:		Power points required.
	Read_Power_Input("BIO", 40),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List2,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassBarracks(
	STRUCT_BARRACKS,
	TXT_BARRACKS,							// NAME:			Short name of the structure.
	"PYLE",									// NAME:			Short name of the structure.
	XYP_COORD(30,33),						// Exit point for produced units.
	Read_Build_Level("PYLE", 0),								// Build level.
	Read_Prerequisite("PYLE", STRUCT_POWER),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,			// Is it a factory type building?
		Read_Is_Capturable("PYLE", true),			// Can this building be captured?
		Read_Is_Flammable("PYLE", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("PYLE", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_INFANTRYTYPE,					// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("PYLE", 400),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("PYLE", 300),										// COST:			Cost to purchase.
	Read_Scenario_Level("PYLE", 1),											// SCENARIO:	Starting availability scenario.
	0,60,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("PYLE", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY",
	Read_Primary_Weapon("PYLE", WEAPON_NONE),Read_Secondary_Weapon("PYLE", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("PYLE", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("PYLE", 0),											// POWER:		Power points required.
	Read_Power_Input("PYLE", 20),										// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	(short const *)ExitPyle,			// Preferred exit cell list.
	(short const *)List22_1100,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)List22_0011			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassHand(
	STRUCT_HAND,
	TXT_HAND,								// NAME:			Short name of the structure.
	"HAND",									// NAME:			Short name of the structure.
	XYP_COORD(36,63),						// Exit point for produced units.
	Read_Build_Level("HAND", 0),								// Build level.
	Read_Prerequisite("HAND", STRUCT_POWER),				// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		true,			// Is it a factory type building?
		Read_Is_Capturable("HAND", true),			// Can this building be captured?
		Read_Is_Flammable("HAND", false),		// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("HAND", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_INFANTRYTYPE,					// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("HAND", 400),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("HAND", 300),										// COST:			Cost to purchase.
	Read_Scenario_Level("HAND", 2),											// SCENARIO:	Starting availability scenario.
	0,61,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("HAND", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY",
	Read_Primary_Weapon("HAND", WEAPON_NONE),Read_Secondary_Weapon("HAND", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("HAND", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("HAND", 0),											// POWER:		Power points required.
	Read_Power_Input("HAND", 20),										// DRAIN:		Power points required.
	BSIZE_23, 								// SIZE:			Building size.
	(short const *)ExitHand,		// Preferred exit cell list.
	(short const *)ListHand,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListHand		// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassTanker(
	STRUCT_TANKER,
	TXT_TANKER,								// NAME:			Short name of the structure.
	"ARCO",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("ARCO", 99),							// Build level.
	Read_Prerequisite("ARCO", STRUCT_POWER),							// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("ARCO", false),		// Can this building be captured?
		Read_Is_Flammable("ARCO", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("ARCO", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("ARCO", 100),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("ARCO", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("ARCO", 0),											// SCENARIO:	Starting availability scenario.
	0,1, 										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("ARCO", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("ARCO", WEAPON_NONE),Read_Secondary_Weapon("ARCO", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("ARCO", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("ARCO", 0),											// POWER:		Power points required.
	Read_Power_Input("ARCO", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List21,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassRepair(
	STRUCT_REPAIR,
	TXT_FIX_IT,								// NAME:			Short name of the structure.
	"FIX",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("FIX", 5),								// Build level.
	Read_Prerequisite("FIX", STRUCT_POWER),							// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		false,		// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("FIX", true),			// Can this building be captured?
		Read_Is_Flammable("FIX", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("FIX", true),			// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("FIX", 400),										// STRNTH:		Full strength of building.
	3,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("FIX", 1200),										// COST:			Cost to purchase.
	Read_Scenario_Level("FIX", 8),											// SCENARIO:	Starting availability scenario.
	0,46,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("FIX", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("FIX", WEAPON_NONE),Read_Secondary_Weapon("FIX", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("FIX", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("FIX", 0),											// POWER:		Power points required.
	Read_Power_Input("FIX", 30),										// DRAIN:		Power points required.
	BSIZE_33, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)ListFix,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListFix		// OVERLAPLIST:List of overlap cell offset.
);

#ifdef OBSOLETE
static BuildingTypeClass const ClassRoad(
	STRUCT_ROAD,
	TXT_ROAD,								// NAME:			Short name of the structure.
	"ROAD",									// NAME:			Short name of the structure.
	Read_Prerequisite("ROAD", STRUCT_NONE),							// PREREQ:		Buildings that must exist first.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("ROAD", 99),							// Build level.
	0,								// Building prerequisite.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("ROAD", false),		// Can this building be captured?
		Read_Is_Flammable("ROAD", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		false,		// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("ROAD", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("ROAD", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("ROAD", 50),										// COST:			Cost to purchase.
	Read_Scenario_Level("ROAD", 99),										// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("ROAD", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("ROAD", WEAPON_NONE),Read_Secondary_Weapon("ROAD", WEAPON_NONE),
	ARMOR_NONE,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("ROAD", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("ROAD", 0),											// POWER:		Power points required.
	Read_Power_Input("ROAD", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL			// OVERLAPLIST:List of overlap cell offset.
);
#endif

static BuildingTypeClass const ClassV01(
	STRUCT_V01,
	TXT_CIV1,								// NAME:			Short name of the structure.
	"V01",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V01", 99),							// Build level.
	Read_Prerequisite("V01", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V01", false),		// Can this building be captured?
		Read_Is_Flammable("V01", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V01", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V01", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V01", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V01", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V01", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V01", WEAPON_NONE),Read_Secondary_Weapon("V01", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V01", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V01", 0),											// POWER:		Power points required.
	Read_Power_Input("V01", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV02(
	STRUCT_V02,
	TXT_CIV2,								// NAME:			Short name of the structure.
	"V02",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V02", 99),							// Build level.
	Read_Prerequisite("V02", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V02", false),		// Can this building be captured?
		Read_Is_Flammable("V02", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V02", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V02", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V02", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V02", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V02", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V02", WEAPON_NONE),Read_Secondary_Weapon("V02", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V02", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V02", 0),											// POWER:		Power points required.
	Read_Power_Input("V02", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV03(
	STRUCT_V03,
	TXT_CIV3,								// NAME:			Short name of the structure.
	"V03",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V03", 99),							// Build level.
	Read_Prerequisite("V03", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V03", false),		// Can this building be captured?
		Read_Is_Flammable("V03", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V03", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V03", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V03", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V03", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V03", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V03", WEAPON_NONE),Read_Secondary_Weapon("V03", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V03", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V03", 0),											// POWER:		Power points required.
	Read_Power_Input("V03", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0111,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1000				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV04(
	STRUCT_V04,
	TXT_CIV4,								// NAME:			Short name of the structure.
	"V04",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V04", 99),							// Build level.
	Read_Prerequisite("V04", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V04", false),		// Can this building be captured?
		Read_Is_Flammable("V04", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V04", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V04", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V04", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V04", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V04", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V04", WEAPON_NONE),Read_Secondary_Weapon("V04", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V04", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V04", 0),											// POWER:		Power points required.
	Read_Power_Input("V04", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV05(
	STRUCT_V05,
	TXT_CIV5,								// NAME:			Short name of the structure.
	"V05",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V05", 99),							// Build level.
	Read_Prerequisite("V05", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V05", false),		// Can this building be captured?
		Read_Is_Flammable("V05", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V05", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V05", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V05", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V05", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V05", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V05", WEAPON_NONE),Read_Secondary_Weapon("V05", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V05", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V05", 0),											// POWER:		Power points required.
	Read_Power_Input("V05", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV06(
	STRUCT_V06,
	TXT_CIV6,								// NAME:			Short name of the structure.
	"V06",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V06", 99),							// Build level.
	Read_Prerequisite("V06", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V06", false),		// Can this building be captured?
		Read_Is_Flammable("V06", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V06", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V06", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V06", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V06", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V06", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V06", WEAPON_NONE),Read_Secondary_Weapon("V06", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V06", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V06", 0),											// POWER:		Power points required.
	Read_Power_Input("V06", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV07(
	STRUCT_V07,
	TXT_CIV7,								// NAME:			Short name of the structure.
	"V07",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V07", 99),							// Build level.
	Read_Prerequisite("V07", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V07", false),		// Can this building be captured?
		Read_Is_Flammable("V07", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V07", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V07", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V07", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V07", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V07", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V07", WEAPON_NONE),Read_Secondary_Weapon("V07", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V07", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V07", 0),											// POWER:		Power points required.
	Read_Power_Input("V07", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV08(
	STRUCT_V08,
	TXT_CIV8,								// NAME:			Short name of the structure.
	"V08",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V08", 99),							// Build level.
	Read_Prerequisite("V08", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V08", false),		// Can this building be captured?
		Read_Is_Flammable("V08", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V08", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V08", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V08", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V08", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V08", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V08", WEAPON_NONE),Read_Secondary_Weapon("V08", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V08", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V08", 0),											// POWER:		Power points required.
	Read_Power_Input("V08", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV09(
	STRUCT_V09,
	TXT_CIV9,								// NAME:			Short name of the structure.
	"V09",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V09", 99),							// Build level.
	Read_Prerequisite("V09", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V09", false),		// Can this building be captured?
		Read_Is_Flammable("V09", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V09", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V09", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V09", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V09", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V09", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V09", WEAPON_NONE),Read_Secondary_Weapon("V09", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V09", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V09", 0),											// POWER:		Power points required.
	Read_Power_Input("V09", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV10(
	STRUCT_V10,
	TXT_CIV10,								// NAME:			Short name of the structure.
	"V10",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V10", 99),							// Build level.
	Read_Prerequisite("V10", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V10", false),		// Can this building be captured?
		Read_Is_Flammable("V10", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V10", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V10", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V10", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V10", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V10", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V10", WEAPON_NONE),Read_Secondary_Weapon("V10", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V10", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V10", 0),											// POWER:		Power points required.
	Read_Power_Input("V10", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV11(
	STRUCT_V11,
	TXT_CIV11,								// NAME:			Short name of the structure.
	"V11",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V11", 99),							// Build level.
	Read_Prerequisite("V11", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V11", false),		// Can this building be captured?
		Read_Is_Flammable("V11", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V11", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V11", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V11", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V11", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V11", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V11", WEAPON_NONE),Read_Secondary_Weapon("V11", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V11", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V11", 0),											// POWER:		Power points required.
	Read_Power_Input("V11", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV12(
	STRUCT_V12,
	TXT_CIV12,								// NAME:			Short name of the structure.
	"V12",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V12", 99),							// Build level.
	Read_Prerequisite("V12", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V12", false),		// Can this building be captured?
		Read_Is_Flammable("V12", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V12", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V12", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V12", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V12", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V12", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V12", WEAPON_NONE),Read_Secondary_Weapon("V12", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V12", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V12", 0),											// POWER:		Power points required.
	Read_Power_Input("V12", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV13(
	STRUCT_V13,
	TXT_CIV13,								// NAME:			Short name of the structure.
	"V13",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V13", 99),							// Build level.
	Read_Prerequisite("V13", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V13", false),		// Can this building be captured?
		Read_Is_Flammable("V13", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V13", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V13", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V13", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V13", 0),											// SCENARIO:	Starting availability scenario.
	0,2, 						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V13", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V13", WEAPON_NONE),Read_Secondary_Weapon("V13", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V13", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V13", 0),											// POWER:		Power points required.
	Read_Power_Input("V13", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV14(
	STRUCT_V14,
	TXT_CIV14,								// NAME:			Short name of the structure.
	"V14",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V14", 99),							// Build level.
	Read_Prerequisite("V14", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V14", false),		// Can this building be captured?
		Read_Is_Flammable("V14", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V14", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V14", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V14", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V14", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V14", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V14", WEAPON_NONE),Read_Secondary_Weapon("V14", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V14", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V14", 0),											// POWER:		Power points required.
	Read_Power_Input("V14", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV15(
	STRUCT_V15,
	TXT_CIV15,								// NAME:			Short name of the structure.
	"V15",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V15", 99),							// Build level.
	Read_Prerequisite("V15", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V15", false),		// Can this building be captured?
		Read_Is_Flammable("V15", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V15", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V15", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V15", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V15", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V15", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V15", WEAPON_NONE),Read_Secondary_Weapon("V15", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V15", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V15", 0),											// POWER:		Power points required.
	Read_Power_Input("V15", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV16(
	STRUCT_V16,
	TXT_CIV16,								// NAME:			Short name of the structure.
	"V16",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V16", 99),							// Build level.
	Read_Prerequisite("V16", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V16", false),		// Can this building be captured?
		Read_Is_Flammable("V16", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V16", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V16", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V16", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V16", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V16", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V16", WEAPON_NONE),Read_Secondary_Weapon("V16", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V16", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V16", 0),											// POWER:		Power points required.
	Read_Power_Input("V16", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV17(
	STRUCT_V17,
	TXT_CIV17,								// NAME:			Short name of the structure.
	"V17",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V17", 99),							// Build level.
	Read_Prerequisite("V17", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V17", false),		// Can this building be captured?
		Read_Is_Flammable("V17", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V17", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V17", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V17", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V17", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V17", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V17", WEAPON_NONE),Read_Secondary_Weapon("V17", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V17", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V17", 0),											// POWER:		Power points required.
	Read_Power_Input("V17", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV18(
	STRUCT_V18,
	TXT_CIV18,								// NAME:			Short name of the structure.
	"V18",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V18", 99),							// Build level.
	Read_Prerequisite("V18", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V18", false),		// Can this building be captured?
		Read_Is_Flammable("V18", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		true,			// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V18", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V18", 200),										// STRNTH:		Full strength of building.
	1,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V18", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V18", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V18", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL"),						// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL",
	Read_Primary_Weapon("V18", WEAPON_NONE),Read_Secondary_Weapon("V18", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V18", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V18", 0),											// POWER:		Power points required.
	Read_Power_Input("V18", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV19(
	STRUCT_PUMP,
	TXT_PUMP,								// NAME:			Short name of the structure.
	"V19",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V19", 99),							// Build level.
	Read_Prerequisite("V19", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V19", false),		// Can this building be captured?
		Read_Is_Flammable("V19", true),			// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V19", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V19", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V19", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V19", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V19", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V19", WEAPON_NONE),Read_Secondary_Weapon("V19", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V19", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V19", 0),											// POWER:		Power points required.
	Read_Power_Input("V19", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV20(
	STRUCT_V20,
	TXT_CIV20,								// NAME:			Short name of the structure.
	"V20",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V20", 99),							// Build level.
	Read_Prerequisite("V20", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V20", false),		// Can this building be captured?
		Read_Is_Flammable("V20", true),			// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V20", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V20", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V20", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V20", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V20", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V20", WEAPON_NONE),Read_Secondary_Weapon("V20", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V20", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V20", 0),											// POWER:		Power points required.
	Read_Power_Input("V20", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV21(
	STRUCT_V21,
	TXT_CIV21,								// NAME:			Short name of the structure.
	"V21",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V21", 99),							// Build level.
	Read_Prerequisite("V21", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V21", false),		// Can this building be captured?
		Read_Is_Flammable("V21", true),			// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V21", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V21", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V21", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V21", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V21", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V21", WEAPON_NONE),Read_Secondary_Weapon("V21", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V21", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V21", 0),											// POWER:		Power points required.
	Read_Power_Input("V21", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1101,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List0010				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV22(
	STRUCT_V22,
	TXT_CIV22,								// NAME:			Short name of the structure.
	"V22",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V22", 99),							// Build level.
	Read_Prerequisite("V22", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V22", false),		// Can this building be captured?
		Read_Is_Flammable("V22", true),			// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V22", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V22", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V22", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V22", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V22", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V22", WEAPON_NONE),Read_Secondary_Weapon("V22", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V22", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V22", 0),											// POWER:		Power points required.
	Read_Power_Input("V22", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV23(
	STRUCT_V23,
	TXT_CIV23,								// NAME:			Short name of the structure.
	"V23",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V23", 99),							// Build level.
	Read_Prerequisite("V23", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V23", false),		// Can this building be captured?
		Read_Is_Flammable("V23", true),			// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V23", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V23", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V23", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V23", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V23", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V23", WEAPON_NONE),Read_Secondary_Weapon("V23", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V23", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V23", 0),											// POWER:		Power points required.
	Read_Power_Input("V23", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV24(
	STRUCT_V24,
	TXT_CIV24,								// NAME:			Short name of the structure.
	"V24",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V24", 99),							// Build level.
	Read_Prerequisite("V24", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V24", false),		// Can this building be captured?
		Read_Is_Flammable("V24", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V24", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V24", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V24", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V24", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V24", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V24", WEAPON_NONE),Read_Secondary_Weapon("V24", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V24", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V24", 0),											// POWER:		Power points required.
	Read_Power_Input("V24", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0011,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1100				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV25(
	STRUCT_V25,
	TXT_CIV25,								// NAME:			Short name of the structure.
	"V25",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V25", 99),							// Build level.
	Read_Prerequisite("V25", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V25", false),		// Can this building be captured?
		Read_Is_Flammable("V25", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V25", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V25", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V25", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V25", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V25", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V25", WEAPON_NONE),Read_Secondary_Weapon("V25", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V25", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V25", 0),											// POWER:		Power points required.
	Read_Power_Input("V25", 0),											// DRAIN:		Power points required.
	BSIZE_22, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List0111,			// OCCUPYLIST:	List of active foundation squares.
	(short const *)List1000				// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV26(
	STRUCT_V26,
	TXT_CIV26,								// NAME:			Short name of the structure.
	"V26",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V26", 99),							// Build level.
	Read_Prerequisite("V26", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V26", false),		// Can this building be captured?
		Read_Is_Flammable("V26", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V26", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V26", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V26", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V26", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V26", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V26", WEAPON_NONE),Read_Secondary_Weapon("V26", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V26", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V26", 0),											// POWER:		Power points required.
	Read_Power_Input("V26", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV27(
	STRUCT_V27,
	TXT_CIV27,								// NAME:			Short name of the structure.
	"V27",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V27", 99),							// Build level.
	Read_Prerequisite("V27", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V27", false),		// Can this building be captured?
		Read_Is_Flammable("V27", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V27", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V27", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V27", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V27", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V27", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V27", WEAPON_NONE),Read_Secondary_Weapon("V27", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V27", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V27", 0),											// POWER:		Power points required.
	Read_Power_Input("V27", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV28(
	STRUCT_V28,
	TXT_CIV28,								// NAME:			Short name of the structure.
	"V28",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V28", 99),							// Build level.
	Read_Prerequisite("V28", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V28", false),		// Can this building be captured?
		Read_Is_Flammable("V28", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V28", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V28", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V28", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V28", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V28", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V28", WEAPON_NONE),Read_Secondary_Weapon("V28", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V28", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V28", 0),											// POWER:		Power points required.
	Read_Power_Input("V28", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV29(
	STRUCT_V29,
	TXT_CIV29,								// NAME:			Short name of the structure.
	"V29",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V29", 99),							// Build level.
	Read_Prerequisite("V29", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V29", false),		// Can this building be captured?
		Read_Is_Flammable("V29", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V29", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V29", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V29", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V29", 0),											// SCENARIO:	Starting availability scenario.
	0,2, 						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V29", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V29", WEAPON_NONE),Read_Secondary_Weapon("V29", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V29", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V29", 0),											// POWER:		Power points required.
	Read_Power_Input("V29", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV30(
	STRUCT_V30,
	TXT_CIV30,								// NAME:			Short name of the structure.
	"V30",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V30", 99),							// Build level.
	Read_Prerequisite("V30", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V30", false),		// Can this building be captured?
		Read_Is_Flammable("V30", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V30", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V30", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V30", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V30", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V30", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V30", WEAPON_NONE),Read_Secondary_Weapon("V30", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V30", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V30", 0),											// POWER:		Power points required.
	Read_Power_Input("V30", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV31(
	STRUCT_V31,
	TXT_CIV31,								// NAME:			Short name of the structure.
	"V31",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V31", 99),							// Build level.
	Read_Prerequisite("V31", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V31", false),		// Can this building be captured?
		Read_Is_Flammable("V31", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V31", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V31", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V31", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V31", 0),											// SCENARIO:	Starting availability scenario.
	0,2, 						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V31", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V31", WEAPON_NONE),Read_Secondary_Weapon("V31", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V31", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V31", 0),											// POWER:		Power points required.
	Read_Power_Input("V31", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV32(
	STRUCT_V32,
	TXT_CIV32,								// NAME:			Short name of the structure.
	"V32",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V32", 99),							// Build level.
	Read_Prerequisite("V32", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V32", false),		// Can this building be captured?
		Read_Is_Flammable("V32", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V32", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V32", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V32", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V32", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V32", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V32", WEAPON_NONE),Read_Secondary_Weapon("V32", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V32", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V32", 0),											// POWER:		Power points required.
	Read_Power_Input("V32", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV33(
	STRUCT_V33,
	TXT_CIV33,								// NAME:			Short name of the structure.
	"V33",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V33", 99),							// Build level.
	Read_Prerequisite("V33", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V33", false),		// Can this building be captured?
		Read_Is_Flammable("V33", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V33", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V33", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V33", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V33", 0),											// SCENARIO:	Starting availability scenario.
	0,2,						// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V33", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V33", WEAPON_NONE),Read_Secondary_Weapon("V33", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V33", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V33", 0),											// POWER:		Power points required.
	Read_Power_Input("V33", 0),											// DRAIN:		Power points required.
	BSIZE_21, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List11,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV34(
	STRUCT_V34,
	TXT_CIV34,								// NAME:			Short name of the structure.
	"V34",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V34", 99),							// Build level.
	Read_Prerequisite("V34", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V34", false),		// Can this building be captured?
		Read_Is_Flammable("V34", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V34", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V34", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V34", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V34", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V34", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V34", WEAPON_NONE),Read_Secondary_Weapon("V34", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V34", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V34", 0),											// POWER:		Power points required.
	Read_Power_Input("V34", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV35(
	STRUCT_V35,
	TXT_CIV35,								// NAME:			Short name of the structure.
	"V35",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V35", 99),							// Build level.
	Read_Prerequisite("V35", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V35", false),		// Can this building be captured?
		Read_Is_Flammable("V35", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V35", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V35", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V35", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V35", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V35", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V35", WEAPON_NONE),Read_Secondary_Weapon("V35", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V35", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V35", 0),											// POWER:		Power points required.
	Read_Power_Input("V35", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

static BuildingTypeClass const ClassV36(
	STRUCT_V36,
	TXT_CIV36,								// NAME:			Short name of the structure.
	"V36",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V36", 99),							// Build level.
	Read_Prerequisite("V36", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V36", false),		// Can this building be captured?
		Read_Is_Flammable("V36", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V36", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V36", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V36", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V36", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V36", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V36", WEAPON_NONE),Read_Secondary_Weapon("V36", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V36", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V36", 0),											// POWER:		Power points required.
	Read_Power_Input("V36", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassV37(
	STRUCT_V37,
	TXT_CIV37,								// NAME:			Short name of the structure.
	"V37",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("V37", 99),							// Build level.
	Read_Prerequisite("V37", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("V37", false),		// Can this building be captured?
		Read_Is_Flammable("V37", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		true,			// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("V37", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("V37", 300),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("V37", 0),											// COST:			Cost to purchase.
	Read_Scenario_Level("V37", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("V37", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("V37", WEAPON_NONE),Read_Secondary_Weapon("V37", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("V37", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("V37", 0),											// POWER:		Power points required.
	Read_Power_Input("V37", 0),											// DRAIN:		Power points required.
	BSIZE_42, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)ListWestwood,		// OCCUPYLIST:	List of active foundation squares.
	(short const *)OListWestwood		// OVERLAPLIST:List of overlap cell offset.
);
static BuildingTypeClass const ClassMission(
	STRUCT_MISSION,
	TXT_CIVMISS,							// NAME:			Short name of the structure.
	"MISS",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("MISS", 99),							// Build level.
	Read_Prerequisite("MISS", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		true,			// Has ability to detect adjacent cloaked objects?
		true,			// Animation rate is regulated for constant speed?
		true,			// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		false,		// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("MISS", true),			// Can this building be captured?
		Read_Is_Flammable("MISS", true),			// Does it catch fire?
		true,			// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		true,			// Can the player select this?
		true,			// Is this a legal target for attack or move?
		false,		// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		true,			// Can it be repaired?
		Read_Is_Buildable("MISS", false),		// Can it be manufactured by the player?
		true,			// Does it contain a crew?
		false,		// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("MISS", 200),										// STRNTH:		Full strength of building.
	2,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("MISS", 1000),										// COST:			Cost to purchase.
	Read_Scenario_Level("MISS", 0),											// SCENARIO:	Starting availability scenario.
	0,2,									// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("MISS", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("MISS", WEAPON_NONE),Read_Secondary_Weapon("MISS", WEAPON_NONE),
	ARMOR_WOOD,								// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("MISS", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("MISS", 0),											// POWER:		Power points required.
	Read_Power_Input("MISS", 0),											// DRAIN:		Power points required.
	BSIZE_32, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List32,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);

// Sandbag wall
static BuildingTypeClass const Sandbag(
	STRUCT_SANDBAG_WALL,
	TXT_SANDBAG_WALL,						// NAME:			Short name of the structure.
	"SBAG",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("SBAG", 2),								// Build level.
	Read_Prerequisite("SBAG", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		true,			// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("SBAG", false),		// Can this building be captured?
		Read_Is_Flammable("SBAG", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("SBAG", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("SBAG", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("SBAG", 50),										// COST:			Cost to purchase.
	Read_Scenario_Level("SBAG", 5),											// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("SBAG", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("SBAG", WEAPON_NONE),Read_Secondary_Weapon("SBAG", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("SBAG", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("SBAG", 0),											// POWER:		Power points required.
	Read_Power_Input("SBAG", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);
// Cyclone fence
static BuildingTypeClass const Cyclone(
	STRUCT_CYCLONE_WALL,
	TXT_CYCLONE_WALL,						// NAME:			Short name of the structure.
	"CYCL",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("CYCL", 5),								// Build level.
	Read_Prerequisite("CYCL", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		true,			// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("CYCL", false),		// Can this building be captured?
		Read_Is_Flammable("CYCL", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("CYCL", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("CYCL", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("CYCL", 75),										// COST:			Cost to purchase.
	Read_Scenario_Level("CYCL", 9),											// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("CYCL", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_GOOD | HOUSEF_BAD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY"),								// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,GOODGUY,BADGUY",
	Read_Primary_Weapon("CYCL", WEAPON_NONE),Read_Secondary_Weapon("CYCL", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("CYCL", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("CYCL", 0),											// POWER:		Power points required.
	Read_Power_Input("CYCL", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);
// Brick wall
static BuildingTypeClass const Brick(
	STRUCT_BRICK_WALL,
	TXT_BRICK_WALL,						// NAME:			Short name of the structure.
	"BRIK",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("BRIK", 7),								// Build level.
	Read_Prerequisite("BRIK", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		true,			// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("BRIK", false),		// Can this building be captured?
		Read_Is_Flammable("BRIK", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("BRIK", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("BRIK", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BRIK", 100),										// COST:			Cost to purchase.
	Read_Scenario_Level("BRIK", 13),										// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BRIK", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_BAD | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,GOODGUY",
	Read_Primary_Weapon("BRIK", WEAPON_NONE),Read_Secondary_Weapon("BRIK", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("BRIK", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("BRIK", 0),											// POWER:		Power points required.
	Read_Power_Input("BRIK", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);
// Barbwire wall
static BuildingTypeClass const Barbwire(
	STRUCT_BARBWIRE_WALL,
	TXT_BARBWIRE_WALL,					// NAME:			Short name of the structure.
	"BARB",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("BARB", 98),							// Build level.
	Read_Prerequisite("BARB", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		true,			// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("BARB", false),		// Can this building be captured?
		Read_Is_Flammable("BARB", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("BARB", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("BARB", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("BARB", 25),										// COST:			Cost to purchase.
	Read_Scenario_Level("BARB", 98),										// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("BARB", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_BAD | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,BADGUY,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("BARB", WEAPON_NONE),Read_Secondary_Weapon("BARB", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("BARB", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("BARB", 0),											// POWER:		Power points required.
	Read_Power_Input("BARB", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);
// Wood wall
static BuildingTypeClass const Wood(
	STRUCT_WOOD_WALL,
	TXT_WOOD_WALL,							// NAME:			Short name of the structure.
	"WOOD",									// NAME:			Short name of the structure.
	XYP_COORD(0,0),						// Exit point for produced units.
	Read_Build_Level("WOOD", 99),							// Build level.
	Read_Prerequisite("WOOD", STRUCT_NONE),				// PREREQ:		Buildings that must exist first.
		false,		// Has ability to detect adjacent cloaked objects?
		false,		// Animation rate is regulated for constant speed?
		false,		// Requires a bib dirt patch?
		true,			// Always use the given name for the building?
		true,			// Is this a wall type structure?
		false,		// Is it a factory type building?
		Read_Is_Capturable("WOOD", false),		// Can this building be captured?
		Read_Is_Flammable("WOOD", false),		// Does it catch fire?
		false,		// Simple (one frame) damage imagery?
		false,		// Is it invisible to radar?
		false,		// Can the player select this?
		true,			// Is this a legal target for attack or move?
		true,			// Is this an insignificant building?
		false,		// Is it immune to normal combat damage?
		false,		// Theater specific graphic image?
		false,		// Does it have a rotating turret?
		false,		// Fires multiple shots in quick succession?
		false,		// Can it be repaired?
		Read_Is_Buildable("WOOD", true),			// Can it be manufactured by the player?
		false,		// Does it contain a crew?
		true,			// Does building care less if placed on concrete?
	RTTI_NONE,								// The object type produced at this factory.
	DIR_N,									// Starting idle frame to match construction.
	Read_Strength("WOOD", 1),											// STRNTH:		Full strength of building.
	0,											// SIGHTRANGE:	Range of sighting.
	Read_Cost("WOOD", 25),										// COST:			Cost to purchase.
	Read_Scenario_Level("WOOD", 98),										// SCENARIO:	Starting availability scenario.
	0,0,										// RISK/RWRD:	Risk/reward rating values.
	Read_House_List_From_Rules_Ini("WOOD", HOUSEF_MULTI1 | HOUSEF_MULTI2 | HOUSEF_MULTI3 | HOUSEF_MULTI4 | HOUSEF_MULTI5 | HOUSEF_MULTI6 | HOUSEF_JP | HOUSEF_NEUTRAL | HOUSEF_GOOD, "MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY"),							// OWNABLE:		Ownable by house (bit field).
	"MULTI1,MULTI2,MULTI3,MULTI4,MULTI5,MULTI6,JP,NEUTRAL,GOODGUY",
	Read_Primary_Weapon("WOOD", WEAPON_NONE),
	Read_Secondary_Weapon("WOOD", WEAPON_NONE),
	ARMOR_ALUMINUM,						// ARMOR:		Armor type
	0,											// CANENTER:	Units that can enter building.
	Read_Storage_Capacity("WOOD", 0),											// CAPACITY:	Spice storage capacity.
	Read_Power_Output("WOOD", 0),											// POWER:		Power points required.
	Read_Power_Input("WOOD", 0),											// DRAIN:		Power points required.
	BSIZE_11, 								// SIZE:			Building size.
	NULL,										// Preferred exit cell list.
	(short const *)List1,				// OCCUPYLIST:	List of active foundation squares.
	(short const *)NULL					// OVERLAPLIST:List of overlap cell offset.
);


BuildingTypeClass const * const BuildingTypeClass::Pointers[STRUCT_COUNT] = {
	&ClassWeapon,			//	STRUCT_WEAP
	&ClassGTower,			//	STRUCT_GTOWER
	&ClassATower,			//	STRUCT_ATOWER
	&ClassObelisk,			//	STRUCT_OBLISK
	&ClassCommand,			//	STRUCT_RADAR
	&ClassTurret,			//	STRUCT_TURRET
	&ClassConst,			//	STRUCT_CONST
	&ClassRefinery,		//	STRUCT_REFINERY
	&ClassStorage,			//	STRUCT_STORAGE
	&ClassHelipad,			//	STRUCT_HELIPAD
	&ClassSAM,				//	STRUCT_SAM
	&ClassAirStrip,		//	STRUCT_AIRSTRIP
	&ClassPower,			//	STRUCT_POWER
	&ClassAdvancedPower,	//	STRUCT_POWER
	&ClassHospital,		//	STRUCT_HOSPITAL
	&ClassBarracks,		//	STRUCT_BARRACKS
	&ClassTanker,			//	STRUCT_TANKER
	&ClassRepair,			//	STRUCT_REPAIR
	&ClassBioLab,			//	STRUCT_BIO_LAB
	&ClassHand,				//	STRUCT_HAND
	&ClassTemple,			// STRUCT_TEMPLE
	&ClassEye,				//	STRUCT_EYE
	&ClassMission,			//	STRUCT_MISSION

	&ClassV01,				//	STRUCT_V1
	&ClassV02,				//	STRUCT_V2
	&ClassV03,				//	STRUCT_V3
	&ClassV04,				//	STRUCT_V4
	&ClassV05,				//	STRUCT_V5
	&ClassV06,				//	STRUCT_V6
	&ClassV07,				//	STRUCT_V7
	&ClassV08,				//	STRUCT_V8
	&ClassV09,				//	STRUCT_V9
	&ClassV10,				//	STRUCT_V10
	&ClassV11,				//	STRUCT_V11
	&ClassV12,				//	STRUCT_V12
	&ClassV13,				//	STRUCT_V13
	&ClassV14,				//	STRUCT_V14
	&ClassV15,				//	STRUCT_V15
	&ClassV16,				//	STRUCT_V16
	&ClassV17,				//	STRUCT_V17
	&ClassV18,				//	STRUCT_V18
	&ClassV19,				//	STRUCT_PUMP
	&ClassV20,				//	STRUCT_V20
	&ClassV21,				//	STRUCT_V21
	&ClassV22,				//	STRUCT_V22
	&ClassV23,				//	STRUCT_V23
	&ClassV24,				//	STRUCT_V24
	&ClassV25,				//	STRUCT_V25
	&ClassV26,				//	STRUCT_V26
	&ClassV27,				//	STRUCT_V27
	&ClassV28,				//	STRUCT_V28
	&ClassV29,				//	STRUCT_V29
	&ClassV30,				//	STRUCT_V30
	&ClassV31,				//	STRUCT_V31
	&ClassV32,				//	STRUCT_V32
	&ClassV33,				//	STRUCT_V33
	&ClassV34,				//	STRUCT_V34
	&ClassV35,				//	STRUCT_V35
	&ClassV36,				//	STRUCT_V36
	&ClassV37,				//	STRUCT_V37
#ifdef OBSOLETE
	&ClassRoad,				// STRUCT_ROAD
#endif
	&Sandbag,				// STRUCT_SANDBAG_WALL
	&Cyclone,				//	STRUCT_CYCLONE_WALL
	&Brick,					// STRUCT_BRICK_WALL
	&Barbwire,				// STRUCT_BARBWIRE_WALL
	&Wood,					//	STRUCT_WOOD_WALL
};

void const *WarFactoryOverlay;


/***********************************************************************************************
 * BuildingTypeClass::BuildingTypeClass -- This is the constructor for the building types.     *
 *                                                                                             *
 *    This is the constructor used to create the building types.                               *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass::BuildingTypeClass(
						StructType type,
						int name,
						char const *ininame,
						COORDINATE exitpoint,
						unsigned char level,
						long pre,
						bool is_scanner,
						bool is_regulated,
						bool is_bibbed,
						bool is_nominal,
						bool is_wall,
						bool is_factory,
						bool is_captureable,
						bool is_flammable,
						bool is_simpledamage,
						bool is_stealthy,
						bool is_selectable,
						bool is_legal_target,
						bool is_insignificant,
						bool is_immune,
						bool is_theater,
						bool is_turret_equipped,
						bool is_twoshooter,
						bool is_repairable,
						bool is_buildable,
						bool is_crew,
						bool is_sturdy,
						RTTIType tobuild,
						DirType sframe,
						unsigned short strength,
						int sightrange,
						int cost,
						int scenario,
						int risk,
						int reward,
						int ownable,
						const char* houseListCsv,
						WeaponType primary,
						WeaponType secondary,
						ArmorType armor,
						unsigned long canenter,
						unsigned capacity,
						int power,
						int drain,
						BSizeType size,
						short const *exitlist,
						short const *sizelist,
						short const *overlap,
						bool is_unsellable) :
									TechnoTypeClass(name,
													ininame,
													level,
													pre,
													false,
													is_scanner,
													is_nominal,
													false,
													is_flammable,
													false,
													is_stealthy,
													is_selectable,
													is_legal_target,
													is_insignificant,
													is_immune,
													is_theater,
													is_twoshooter,
													is_turret_equipped,
													is_repairable,
													is_buildable,
													is_crew,
													-1,
													strength*2,
													MPH_IMMOBILE,
													sightrange,
													cost,
													scenario,
													risk,
													reward,
													ownable,
													houseListCsv,
													primary,
													secondary,
													armor)
{
	CanEnter = canenter;
	Capacity = capacity;
	Drain = drain;
	ExitList = exitlist;
	ExitPoint = exitpoint;
	IsBibbed = is_bibbed;
	IsCaptureable = is_captureable;
	IsFactory = is_factory;
	IsRegulated = is_regulated;
	IsUnsellable = is_unsellable;
	IsSimpleDamage = is_simpledamage;
	IsSturdy = is_sturdy;
	IsWall = is_wall;
	OccupyList = sizelist;
	OverlapList = overlap;
	Power = power;
	Size = size;
	StartFace = sframe;
	ToBuild = tobuild;
	Type = type;

	Anims[BSTATE_CONSTRUCTION].Start = 0;
	Anims[BSTATE_CONSTRUCTION].Count = 1;
	Anims[BSTATE_CONSTRUCTION].Rate = 0;

	Anims[BSTATE_IDLE].Start = 0;
	Anims[BSTATE_IDLE].Count = 1;
	Anims[BSTATE_IDLE].Rate = 0;

	Anims[BSTATE_ACTIVE].Start = 0;
	Anims[BSTATE_ACTIVE].Count = 1;
	Anims[BSTATE_ACTIVE].Rate = 0;

	Anims[BSTATE_AUX1].Start = 0;
	Anims[BSTATE_AUX1].Count = 1;
	Anims[BSTATE_AUX1].Rate = 0;

	Anims[BSTATE_AUX2].Start = 0;
	Anims[BSTATE_AUX2].Count = 1;
	Anims[BSTATE_AUX2].Rate = 0;
}


/***********************************************************************************************
 * BuildingTypeClass::One_Time -- Performs special one time action for buildings.              *
 *                                                                                             *
 *    This routine is used to do the one time action necessary to handle building type class   *
 *    objects. This entails loading of the building shapes and the brain file used by          *
 *    buildings.                                                                               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine should only be called ONCE.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *   06/11/1994 JLB : Updated construction time and frame count logic.                         *
 *=============================================================================================*/
void BuildingTypeClass::One_Time(void)
{
	static const struct {
		StructType	Class;			// Building class number.
		BStateType	Stage;			// Animation sequence to assign animation range to.
		int			Start;			// Starting frame number.
		int			Length;			// Number of frames (-1 means use all frames).
		int			Rate;				// Rate of animation.
	} _anims[] = {
		{STRUCT_OBELISK,	BSTATE_ACTIVE,			0,	4,	OBELISK_ANIMATION_RATE},
		{STRUCT_ADVANCED_POWER,	BSTATE_IDLE,	0,	4,	15},
		{STRUCT_AIRSTRIP,	BSTATE_IDLE,			0,	16,3},
		{STRUCT_BARRACKS,	BSTATE_ACTIVE,			0,	10,3},
		{STRUCT_BARRACKS,	BSTATE_IDLE,			0,	10,3},
		{STRUCT_CONST,		BSTATE_ACTIVE,			4,	20,3},
		{STRUCT_CONST,		BSTATE_IDLE,			0,	4,	3},
		{STRUCT_EYE,		BSTATE_IDLE,			0,	16,4},
		{STRUCT_HELIPAD,	BSTATE_ACTIVE,			0,	7,	4},
		{STRUCT_HELIPAD,	BSTATE_IDLE,			0,	0,	0},
		{STRUCT_HOSPITAL,	BSTATE_IDLE,			0,	4,	3},
		{STRUCT_POWER,		BSTATE_IDLE,			0,	4,	15},
		{STRUCT_PUMP,		BSTATE_IDLE,			0,	14,4},
		{STRUCT_RADAR,		BSTATE_IDLE,			0,	16,4},
		{STRUCT_REFINERY,	BSTATE_ACTIVE,			12,7,	4},	// Docking phase.
		{STRUCT_REFINERY,	BSTATE_AUX1,			19,5,	4},	// Siphoning phase.
		{STRUCT_REFINERY,	BSTATE_AUX2,			24,6,	4},	// Undocking phase.
		{STRUCT_REFINERY,	BSTATE_IDLE,			0,	6,	4},	// Idle phase.
		{STRUCT_REFINERY,	BSTATE_FULL,			6,	6,	4},	// Flashing lights
		{STRUCT_REPAIR,	BSTATE_ACTIVE,			0,	7,	2},
		{STRUCT_REPAIR,	BSTATE_IDLE,			0,	1,	0},
		{STRUCT_TEMPLE,	BSTATE_IDLE,			0,	1,	0},
		{STRUCT_V20,		BSTATE_IDLE,			0,	3,	3},
		{STRUCT_V21,		BSTATE_IDLE,			0,	3,	3},
		{STRUCT_V22,		BSTATE_IDLE,			0,	3,	3},
		{STRUCT_V23,		BSTATE_IDLE,			0,	3,	3},
		{STRUCT_WEAP,		BSTATE_ACTIVE,			0,	1,	0},
		{STRUCT_WEAP,		BSTATE_IDLE,			0,	1,	0},
		{STRUCT_TEMPLE,	BSTATE_ACTIVE,			0,	5,	1},
	};

	for (StructType sindex = STRUCT_FIRST; sindex < STRUCT_COUNT; sindex++) {
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char	buffer[_MAX_FNAME];
		BuildingTypeClass const & building = As_Reference(sindex);

		/*
		**	Fetch the sidebar cameo image for this building.
		*/
		if (building.IsBuildable) {
			if ( Get_Resolution_Factor() ) {
				sprintf(buffer, "%sICNH", building.IniName);
			} else {
				sprintf(buffer, "%sICON", building.IniName);
			}
			_makepath(fullname, NULL, NULL, buffer, ".SHP");
			((void const *&)building.CameoData) = MixFileClass::Retrieve(fullname);
		}

		/*
		**	Fetch the construction animation for this building.
		*/
		sprintf(buffer, "%sMAKE", building.IniName);
		_makepath(fullname, NULL, NULL, buffer, ".SHP");
		void const * dataptr = MixFileClass::Retrieve(fullname);
		((void const *&)building.BuildupData) = dataptr;
		if (dataptr) {
			int timedelay = 1;
			int count = Get_Build_Frame_Count(dataptr);
			if (count) {
				timedelay = (5 * TICKS_PER_SECOND) / count;
			}
			building.Init_Anim(BSTATE_CONSTRUCTION, 0, count, timedelay);
		}

		/*
		**	Fetch the normal game shape for this building.
		*/
		_makepath(fullname, NULL, NULL, building.IniName, ".SHP");
		((void const *&)building.ImageData) = MixFileClass::Retrieve(fullname);
	}

	// Try to load weap2.shp
	char fullname[_MAX_FNAME+_MAX_EXT];
	_makepath(fullname, NULL, NULL, (char const *)"WEAP2",".SHP");
	WarFactoryOverlay = MixFileClass::Retrieve(fullname);

	/*
	**	Install all the special animation sequences for the different building types.
	*/
	for (unsigned index = 0; index < (sizeof(_anims) / sizeof(_anims[0])); index++) {
		BuildingTypeClass const *b = &As_Reference(_anims[index].Class);
		if (b) {
			b->Init_Anim(_anims[index].Stage, _anims[index].Start, _anims[index].Length, _anims[index].Rate);
		}
	}
}




/***********************************************************************************************
 * Struct_From_Name -- Find BData structure from its name.                                     *
 *                                                                                             *
 *    This routine will convert an ASCII name for a building class into                        *
 *    the actual building class it represents.                                                 *
 *                                                                                             *
 * INPUT:   name  -- ASCII representation of a building class.                                 *
 *                                                                                             *
 * OUTPUT:  Returns with the actual building class number that the string                      *
 *          represents.                                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/07/1992 JLB : Created.                                                                 *
 *   05/02/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
StructType BuildingTypeClass::From_Name(char const *name)
{
	if (name) {
		for (StructType classid = STRUCT_FIRST; classid < STRUCT_COUNT; classid++) {
			if (stricmp(As_Reference(classid).IniName, name) == 0) {
				return(classid);
			}
		}
	}
	return(STRUCT_NONE);
}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * BuildingTypeClass::Display -- Renders a generic view of building.                           *
 *                                                                                             *
 *    This routine is used to display a generic representation of the                          *
 *    building. Typical use of this occurs with the scenario editor.                           *
 *                                                                                             *
 * INPUT:   x,y      -- Coordinate to display the building (centered).                         *
 *                                                                                             *
 *          window   -- The window the building should be rendered                             *
 *                      relative to.                                                           *
 *                                                                                             *
 *          house    -- The house color to use for the building.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Display(int x, int y, WindowNumberType window, HousesType house) const
{
	void const * ptr = Get_Cameo_Data();
	if (!ptr) {
		ptr = Get_Image_Data();
	}
	CC_Draw_Shape(ptr, 0, x, y, window, SHAPE_FADING|SHAPE_CENTER|SHAPE_WIN_REL, HouseTypeClass::As_Reference(house).RemapTable);
}


/***********************************************************************************************
 * BuildingTypeClass::Prep_For_Add -- Prepares scenario editor for adding a                    *
 *                                                                                             *
 *    This routine is used to prepare the scenario editor for the addition                     *
 *    of a building object to the game.                                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/04/1994 JLB : Uses map editing interface routines.                                     *
 *=============================================================================================*/
void BuildingTypeClass::Prep_For_Add(void)
{
	for (StructType index = STRUCT_FIRST; index < STRUCT_COUNT; index++) {
		if (As_Reference(index).Get_Image_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * BuildingTypeClass::Create_And_Place -- Creates and places a building object onto the map.   *
 *                                                                                             *
 *    This routine is used by the scenario editor to create and place buildings on the map.    *
 *                                                                                             *
 * INPUT:   cell     -- The cell that the building is to be placed upon.                       *
 *                                                                                             *
 *          house    -- The owner of the building.                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the building successfully created and placed on the map?                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Create_And_Place(CELL cell, HousesType house) const
{
	BuildingClass * ptr;

	ptr = new BuildingClass(Type, house);
	if (ptr) {
		return(ptr->Unlimbo(Cell_Coord(cell), DIR_N));
	}
	return(false);
}


/***********************************************************************************************
 * BuildingTypeClass::Create_One_Of -- Creates a building of this type.                        *
 *                                                                                             *
 *    This routine will create a building object of this type. The building object is in a     *
 *    limbo state. It is presumed that the building object will be unlimboed at the correct    *
 *    place and time. Typical use is when the building is created in a factory situation       *
 *    and will be placed on the map when construction completes.                               *
 *                                                                                             *
 * INPUT:   house -- Pointer to the house that is to be the owner of the building.             *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the building. If the building could not be created       *
 *          then a NULL is returned.                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/07/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * BuildingTypeClass::Create_One_Of(HouseClass * house) const
{
	HousesType htype = HOUSE_NEUTRAL;
	if (house) {
		htype = house->Class->House;
	}
	return(new BuildingClass(Type, htype));
}


/***********************************************************************************************
 * BuildingTypeClass::Init_Anim -- Initialize an animation control for a building.             *
 *                                                                                             *
 *    This routine will initialize one animation control element for a                         *
 *    specified building. This modifies a "const" class and thus must                          *
 *    perform some strategic casting to get away with this.                                    *
 *                                                                                             *
 * INPUT:   state -- The animation state to apply these data values to.                        *
 *                                                                                             *
 *          start -- Starting frame for the building's animation.                              *
 *                                                                                             *
 *          count -- The number of frames in this animation.                                   *
 *                                                                                             *
 *          rate  -- The countdown timer between animation frames.                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init_Anim(BStateType state, int start, int count, int rate) const
{
	((int &)Anims[state].Start) = start;
	((int &)Anims[state].Count) = count;
	((int &)Anims[state].Rate) = rate;
}


/***********************************************************************************************
 * BuildingTypeClass::Legal_Placement -- Determines if building can be legally placed at pos.  *
 *                                                                                             *
 *    This routine is used to determine if a building can be legally                           *
 *    placed at the specified position. Buildings can only be placed on                        *
 *    unoccupied rock terrain.                                                                 *
 *                                                                                             *
 * INPUT:   pos   -- Position that the building would be placed (up-left)                      *
 *                                                                                             *
 * OUTPUT:  0=illegal, 1=on concrete, -1..-8=part on concrete.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/15/1991 JLB : Created.                                                                 *
 *   04/26/1992 JLB : Concrete and scenario init adjustment.                                   *
 *   05/06/1992 JLB : Good, Bad, and Adjacent checking added.                                  *
 *   08/09/1992 JLB : Determines full or partial concrete foundation.                          *
 *   06/07/1994 JLB : Handles concrete special check.                                          *
 *   06/21/1994 JLB : Converted to building type class member function.                        *
 *=============================================================================================*/
int BuildingTypeClass::Legal_Placement(CELL pos) const
{
	short const	*offset;		// Pointer to cell offset list.

	if (pos == -1) return(false);

#ifdef NEVER
	/*
	**	Concrete has special checking performed to determine legal placement. Concrete
	**	can legally be placed if there is any cell that would be affected by the concrete
	**	placement. Unlike other buildings, only one cell needs to be effective in order
	**	to flag legal placement for the entire "structure".
	*/
	if (Type == STRUCT_CONCRETE_NOD || Type == STRUCT_CONCRETE_GDI) {
		offset = Occupy_List();
		while (*offset != REFRESH_EOL) {
			if (!Map.Cell_Template(pos + (CELL)*offset++)) {
				return(true);
			}
		}

		/*
		**	No squares would be affected by concrete placement so consider legal
		**	placement query to be false.
		*/
		return(false);
	}
#endif

	/*
	**	Normal buildings must check to see that every foundation square is free of
	**	obstacles. If this check passes for all foundation squares, only then does the
	**	routine return that it is legal to place.
	*/
	offset = Occupy_List(true);
	while (*offset != REFRESH_EOL) {
		CELL cell = pos + *offset++;
		if (!Map.In_Radar(cell)) return(false);
		if (!Map[cell].Is_Generally_Clear()) {
			return(false);
		}
	}
	return(true);
}


/***********************************************************************************************
 * BuildingTypeClass::Who_Can_Build_Me -- Determines which factory can create the building.    *
 *                                                                                             *
 *    Use this routine to determine which building is available to build the building. If      *
 *    there are no suitable buildings, then NULL is returned. Typical use of this function is  *
 *    to maintain the construction list sidebar.                                               *
 *                                                                                             *
 * INPUT:   intheory -- If true, then it merely checks for a building of the proper ownership  *
 *                      when determining if construction is allowed. It doesn't consider the   *
 *                      possibility that the construction building is currently busy or not.   *
 *                                                                                             *
 *          legal    -- Should building prerequisite legality checks be performed as well?     *
 *                      For building placements, this is usually false. For sidebar button     *
 *                      adding, this is usually true.                                          *
 *                                                                                             *
 *          house    -- The owner of the building to be built. Only construction buildings of  *
 *                      the same ownership are allowed to build.                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the construction object (building) that can build        *
 *          the building type.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/29/1994 JLB : Created.                                                                 *
 *   01/02/1995 JLB : Scans in reverse order so that later buildings are biased.               *
 *=============================================================================================*/
BuildingClass * BuildingTypeClass::Who_Can_Build_Me(bool intheory, bool legal, HousesType house) const
{
	for (int index = Buildings.Count()-1; index >= 0; index--) {
		BuildingClass * building = Buildings.Ptr(index);

		if (building && !building->IsInLimbo &&
				building->House->Class->House == house &&
				building->Class->ToBuild == RTTI_BUILDINGTYPE &&
				building->Mission != MISSION_DECONSTRUCTION &&
				((1L << building->ActLike) & Ownable) &&
				(!legal || building->House->Can_Build(Type, building->ActLike)) &&
				(intheory || !building->In_Radio_Contact())) {
			return(building);
		}
	}
	return(0);
}


/***********************************************************************************************
 * BuildingTypeClass::Init -- Performs theater specific initialization.                        *
 *                                                                                             *
 *    This routine is used to perform any initialization that is custom per theater.           *
 *    Typically, this is fetching the building shape data for those building types that have   *
 *    theater specific art.                                                                    *
 *                                                                                             *
 * INPUT:   theater  -- The theater to base this initialization on.                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Init(TheaterType theater)
{
	if (theater != LastTheater){
		char fullname[_MAX_FNAME+_MAX_EXT];

		for (StructType sindex = STRUCT_FIRST; sindex < STRUCT_COUNT; sindex++) {
			BuildingTypeClass const *classptr = &As_Reference(sindex);

			if (classptr->IsTheater) {
				_makepath(fullname, NULL, NULL, classptr->IniName, Theaters[theater].Suffix);
				((void const *&)classptr->ImageData) = MixFileClass::Retrieve(fullname);
			}

			if ( Get_Resolution_Factor() ) {
				char buffer[_MAX_FNAME];
				char fullname[_MAX_FNAME+_MAX_EXT];
				void const * cameo_ptr;

				((void const *&)classptr->CameoData) = NULL;

				sprintf(buffer, "%.4sICNH", classptr->IniName);
				_makepath (fullname, NULL, NULL, buffer, Theaters[theater].Suffix);
				cameo_ptr = MixFileClass::Retrieve(fullname);
				if (cameo_ptr){
					((void const *&)classptr->CameoData) = cameo_ptr;
				}
			}
		}
	}
}


/***********************************************************************************************
 * BuildingTypeClass::Dimensions -- Fetches the pixel dimensions of the building.              *
 *                                                                                             *
 *    This routine will fetch the dimensions of the building (in pixels). These dimensions are *
 *    used to render the selection rectangle and the health bar.                               *
 *                                                                                             *
 * INPUT:   width    -- Reference to the pixel width (to be filled in).                        *
 *                                                                                             *
 *          height   -- Reference to the pixel height (to be filled in).                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void BuildingTypeClass::Dimensions(int &width, int &height) const
{
	static struct {
		int Width;
		int Height;
	} _dimensions[BSIZE_COUNT] = {
		{1,1},
		{2,1},
		{1,2},
		{2,2},
		{2,3},
		{3,2},
		{3,3},
		{4,2},
		{5,5}
	};

	width = _dimensions[Size].Width * ICON_PIXEL_W;
	width -= (width/5);
	height = _dimensions[Size].Height * ICON_PIXEL_H;
	height -= (height/5);
}


/***********************************************************************************************
 * BuildingTypeClass::As_Reference -- Fetches reference to the building type specified.        *
 *                                                                                             *
 *    This routine will fetch a reference to the BuildingTypeClass as indicated by the         *
 *    building type number specified.                                                          *
 *                                                                                             *
 * INPUT:   type  -- The building type number to convert into a BuildingTypeClass reference.   *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the building type class as indicated by the            *
 *          parameter.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
BuildingTypeClass const & BuildingTypeClass::As_Reference(StructType type)
{
	return(* Pointers[type]);
}


/***********************************************************************************************
 * BuildingTypeClass::Occupy_List -- Fetches the occupy list for the building.                 *
 *                                                                                             *
 *    Use this routine to fetch the occupy list pointer for the building. The occupy list is   *
 *    used to determine what cells the building occupies and thus precludes other buildings    *
 *    or objects from using.                                                                   *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to a cell offset list to be used to determine what cells    *
 *          this building occupies.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * BuildingTypeClass::Occupy_List(bool placement) const
{
	SmudgeType bib = SMUDGE_NONE;
	CELL cell=0;

	if ((placement && Bib_And_Offset(bib, cell)) || (Special.IsRoad && (*this == STRUCT_BARRACKS || (placement && *this == STRUCT_REFINERY)))) {

		/*
		**	The barracks is always considered to have a bib under it for placement reasons even
		**	if the bib logic is turned off.
		*/
		if (Special.IsRoad && *this == STRUCT_BARRACKS) {
			bib = SMUDGE_BIB3;
			cell = 0;
		}

		/*
		**	If bibs are disabled, then always ensure that the refinery bib is marked
		**	as occupied.
		*/
		if (Special.IsRoad && *this == STRUCT_REFINERY) {
			bib = SMUDGE_BIB2;
			cell = MAP_CELL_W;
		}

		SmudgeTypeClass const & smudge = SmudgeTypeClass::As_Reference(bib);
		static short _list[25];
		short * dest = &_list[0];

		/*
		**	Copy the bib overlap list into the working buffer.
		*/
		short const * src = smudge.Occupy_List();
		while (*src != REFRESH_EOL) {
			*dest++ = (*src++) + cell;
		}

		/*
		**	Append the building occupy list to this working buffer.
		*/
		src = OccupyList;
		while (src && *src != REFRESH_EOL) {
			*dest++ = *src++;
		}
		*dest = REFRESH_EOL;

		return(&_list[0]);
	}

	if (OccupyList) {
		return(OccupyList);
	}

	static short const _templap[] = {REFRESH_EOL};
	return(&_templap[0]);
}


/***********************************************************************************************
 * BuildingTypeClass::Overlap_List -- Fetches the overlap list for the building.               *
 *                                                                                             *
 *    This routine will fetch the overlap list for the building. The overlap list is used      *
 *    to determine what cells the building's graphics cover, but is not considered to occupy   *
 *    for movement purposes.                                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the cell offset list that is used to determine the       *
 *          cells that this building overlaps.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
short const * BuildingTypeClass::Overlap_List(void) const
{
	if (OverlapList) {
		return(OverlapList);
	}

	static short const _templap[] = {REFRESH_EOL};
	return(&_templap[0]);
}


/***********************************************************************************************
 * BuildingTypeClass::Width -- Determines width of bulding in icons.                           *
 *                                                                                             *
 *    Use this routine to determine the width of the building type in icons.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building width in icons.                                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Width(void) const
{
	static int width[BSIZE_COUNT] = {
		1,
		2,
		1,
		2,
		2,
		3,
		3,
		4,
		5
	};
	return(width[Size]);
}


/***********************************************************************************************
 * BuildingTypeClass::Height -- Determins the height of the building in icons.                 *
 *                                                                                             *
 *    Use this routine to find the height of the building in icons.                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the building height in icons.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Height(void) const
{
	static int height[BSIZE_COUNT] = {
		1,
		1,
		2,
		2,
		3,
		2,
		3,
		2,
		5
	};
	return(height[Size]);
}


/***********************************************************************************************
 * BuildingTypeClass::Repair_Cost -- Determines cost per "step" of repair.                     *
 *                                                                                             *
 *    Use this routine to determine how much it will cost to repair the building one           *
 *    step. A step is defined as the number of hit points returned from the Repair_Step()      *
 *    function.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the credit cost to repair this building one step.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Repair_Cost(void) const
{
	int cost = (Raw_Cost()*REPAIR_STEP) / MaxStrength;
	cost /= 2;
	cost = MAX(cost, 1);
	cost = Fixed_To_Cardinal(cost, REPAIR_PERCENT);
	return(MAX(cost, 1));
}


/***********************************************************************************************
 * BuildingTypeClass::Repair_Step -- Determines the repair step rate.                          *
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
 *   02/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Repair_Step(void) const
{
	return(REPAIR_STEP);
}


/***********************************************************************************************
 * BuildingTypeClass::Bib_And_Offset -- Determines the bib and appropriate cell offset.        *
 *                                                                                             *
 *    This routine is used to determine what (if any) bib should be used for this building     *
 *    and also the cell offset for the upper left corner of the bib smudge type.               *
 *                                                                                             *
 * INPUT:   bib   -- Reference to the bib that should be used for this building.               *
 *                                                                                             *
 *          cell  -- The cell offset for the upper left corner of the bib. This offset is      *
 *                   relative to the upper left corner of the building.                        *
 *                                                                                             *
 * OUTPUT:  Is a bib required for this building? If the result is true, then the correct       *
 *          bib and cell offset will be filled in.                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool BuildingTypeClass::Bib_And_Offset(SmudgeType & bib, CELL & cell) const
{
	bib = SMUDGE_NONE;

	if (IsBibbed && !Special.IsRoad) {
		switch (Width()) {
			case 2:
				bib = SMUDGE_BIB3;
				break;

			case 3:
				bib = SMUDGE_BIB2;
				break;

			case 4:
				bib = SMUDGE_BIB1;
				break;
		}

		/*
		**	Adjust the bib position for special buildings that have the bib as part
		**	of the building art itself.
		*/
		if (bib != SMUDGE_NONE) {
			cell += ((Height()-1)*MAP_CELL_W);
		}
	}
	return(bib != SMUDGE_NONE);
}


/***********************************************************************************************
 * BuildingTypeClass::Max_Pips -- Determines the maximum pips to display.                      *
 *                                                                                             *
 *    Use this routine to determine the maximum number of pips to display on this building     *
 *    when it is rendered. Typically, this is the tiberium capacity divided by 100.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of pips to display on this building when selected.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/29/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int BuildingTypeClass::Max_Pips(void) const
{
	return(Bound(Capacity/100, 0, 10));
}


/***********************************************************************************************
 * BuildingTypeClass::Full_Name -- Fetches the full name text number.                          *
 *                                                                                             *
 *    This routine will fetch the full name of this building (expressed as a text number).     *
 *    If special civilian names are enabled, then the civilian buildings will show their true  *
 *    name rather than "civilian building".                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the full name of this building.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/29/1995 JLB : Created.                                                                 *
 *   07/17/1995 JLB : Village wells will always have their name displayed.                     *
 *=============================================================================================*/
int BuildingTypeClass::Full_Name(void) const
{
	if (::Scenario == 3 && Type == STRUCT_MISSION) {
		return(TXT_PRISON);
	}
	if (!IsNominal || Special.IsNamed || IsWall || Debug_Map || Type == STRUCT_V23 || Type == STRUCT_V30 || Type == STRUCT_MISSION || Type == STRUCT_BIO_LAB) {
		return(TechnoTypeClass::Full_Name());
	}
	return(TXT_CIVILIAN_BUILDING);
}


int BuildingTypeClass::Raw_Cost(void) const
{
#ifdef PATCH
	/*
	**	Forces the turret cost down to original 250 for old
	**	version games.
	*/
	if (IsV107 && Type == STRUCT_TURRET && GameToPlay != GAME_NORMAL) {
		return(250);
	}
#endif

	int cost = TechnoTypeClass::Raw_Cost();

	if (Type == STRUCT_HELIPAD) {
		cost -= AircraftTypeClass::As_Reference(AIRCRAFT_ORCA).Cost;
	}
	if (Type == STRUCT_REFINERY) {
		cost -= UnitTypeClass::As_Reference(UNIT_HARVESTER).Cost;
	}
	return(cost);
}


int BuildingTypeClass::Cost_Of(void) const
{
	if (Special.IsSeparate && Type == STRUCT_HELIPAD) {
		return(Raw_Cost());
	}

#ifdef PATCH
	/*
	**	Forces the turret cost down to original 250 for old
	**	version games.
	*/
	if (IsV107 && Type == STRUCT_TURRET && GameToPlay != GAME_NORMAL) {
		return(250);
	}
#endif

	return(TechnoTypeClass::Cost_Of());
}
