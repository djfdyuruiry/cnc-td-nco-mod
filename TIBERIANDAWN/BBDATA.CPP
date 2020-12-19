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

/* $Header:   F:\projects\c&c\vcs\code\bbdata.cpv   2.17   16 Oct 1995 16:49:46   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : BBDATA.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 23, 1994                                                 *
 *                                                                                             *
 *                  Last Update : October 17, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   BulletTypeClass::BulletTypeClass -- Constructor for bullet type objects.                  *
 *   BulletTypeClass::Load_Shapes -- Load shape data for bullet types.                         *
 *   BulletTypeClass::One_Time -- Performs the one time processing for bullets.                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

/***************************************************************************
**	Detailed information about each class of bullet (projectile) in the game.
*/
static BulletTypeClass const ClassSniper(
	BULLET_BULLET,
	"SNIPER",						// NAME:			Text name of this unit type.
	"50cal",
	"Sniper",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HOLLOW_POINT,	// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFF					// Explosion to use upon impact.
);

static BulletTypeClass const ClassBullet(
	BULLET_BULLET,
	"BULLET",						// NAME:			Text name of this unit type.
	"50cal",
	"M16/Pistol",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_SA,					// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFF					// Explosion to use upon impact.
);

static BulletTypeClass const ClassSpreadfire(
	BULLET_SPREADFIRE,
	"SPREADFIRE",						// NAME:			Text name of this unit type.
	"50cal",
	"Chain Gun Bullet",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_PIFFPIFF				// Explosion to use upon impact.
);

static BulletTypeClass const ClassAPDS(
	BULLET_APDS,
	"APDS",						// NAME:			Text name of this unit type.
	"120mm",
	"Armor Piercing",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT3				// Explosion to use upon impact.
);

static BulletTypeClass const Class120mm(
	BULLET_HE,
	"ARTILLERY_SHELL",						// NAME:			Text name of this unit type.
	"120mm",
	"Artillery",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		true,						// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_MEDIUM_FAST,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_ART_EXP1				// Explosion to use upon impact.
);

static BulletTypeClass const ClassMissile(
	BULLET_SSM,
	"SS_MISSILE",					// NAME:			Text name of this unit type.
	"DRAGON",
	"Surface-to-Surface Missile",
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	7,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	5,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassMissile2(
	BULLET_SSM2,
	"MLRS_ROCKET",					// NAME:			Text name of this unit type.
	"DRAGON",
	"Surface-to-Surface Missile (MLRS)",
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	9,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	7,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassPatriot(
	BULLET_SAM,
	"SAM_MISSILE",					// NAME:			Text name of this unit type.
	"MISSILE",
	"Surface-to-Air Missile (SAM Site)",
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		true,						// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	10,							// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT1				// Explosion to use upon impact.
);

static BulletTypeClass const ClassDragon(
	BULLET_TOW,
	"TOW_MISSILE",					// NAME:			Text name of this unit type.
	"DRAGON",
	"Bazooka",
		true,						// Flies over tall walls?
		true,						// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		true,						// Good against aircraft?
	3,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_ROCKET,					// SPEED:		Miles per hour.
	5,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_AP,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassFlame(
	BULLET_FLAME,
	"FLAME",						// NAME:			Text name of this unit type.
	"FLAME",
	"Flamethrower/Flame Tank",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	12,							// ARMING:		Time to arm projectile after launch.
	12,							// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE, 				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassChem(
	BULLET_CHEMSPRAY,
	"CHEMICALS",						// NAME:			Text name of this unit type.
	"FLAME",
	"Chem. Warrior Spray",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	12,							// ARMING:		Time to arm projectile after launch.
	12,							// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE, 				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNapalm(
	BULLET_NAPALM,
	"NAPALM_BOMB",					// NAME:			Text name of this unit type.
	"BOMBLET",
	"Airstrike Bomb",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		true,						// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		false,					// Good against aircraft?
	24,							// ARMING:		Time to arm projectile after launch.
	24,							// RANGE:		Inherent override range factor.
	MPH_MEDIUM_SLOW,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NAPALM2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassGrenade(
	BULLET_GRENADE,
	"GRENADE_BOMB",						// NAME:			Text name of this unit type.
	"BOMB",
	"Grenade",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		true,						// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		true,						// Is projectile inherently inaccurate?
		true,						// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_MEDIUM_SLOW,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_VEH_HIT2				// Explosion to use upon impact.
);

static BulletTypeClass const ClassLaser(
	BULLET_LASER,
	"LASER_BEAM",						// NAME:			Text name of this unit type.
	"Laser",
	"Obelisk Laser Beam",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_LASER,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNukeUp(
	BULLET_NUKE_UP,
	"NUKE_UP",					// NAME:			Text name of this unit type.
	"ATOMICUP",
	"Nuclear Missile (Going up)",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_FRAG1					// Explosion to use upon impact.
);

static BulletTypeClass const ClassNukeDown(
	BULLET_NUKE_DOWN,
	"NUKE_DOWN",					// NAME:			Text name of this unit type.
	"ATOMICDN",
	"Nuclear Missile (Going down)",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_VERY_FAST,				// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HE,					// WARHEAD:		If fires weapon, warhead type
	ANIM_ATOM_BLAST			// Explosion to use upon impact.
);

static BulletTypeClass const ClassHonestJohn(
	BULLET_HONEST_JOHN,
	"HONEST_JOHN_MISSILE",					// NAME:			Text name of this unit type.
	"MISSILE",
	"Honest John Missile",
		true,						// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		false,					// Is this projectile invisible?
		true,						// Will it blow up even if it gets just NEAR to target?
		true,						// Does it have flickering flame animation?
		true,						// Can it run out of fuel?
		false,					// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	10,							// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_FAST,					// SPEED:		Miles per hour.
	10,							// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FIRE,				// WARHEAD:		If fires weapon, warhead type
	ANIM_NAPALM3				// Explosion to use upon impact.
);

static BulletTypeClass const ClassHeadButt(
	BULLET_HEADBUTT,
	"GORE",						// NAME:			Text name of this unit type.
	"GORE",
	"Dino Ram",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_HEADBUTT,			// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);

static BulletTypeClass const ClassTRexBite(
	BULLET_TREXBITE,
	"CHEW",						// NAME:			Text name of this unit type.
	"CHEW",
	"Dino Bite",
		false,					// Flies over tall walls?
		false,					// Homes in on target?
		false,					// Projectile arcs to the target?
		false,					// Is this a dropping bomb-like object?
		true,						// Is this projectile invisible?
		false,					// Will it blow up even if it gets just NEAR to target?
		false,					// Does it have flickering flame animation?
		false,					// Can it run out of fuel?
		true,						// Is there no visual difference between projectile facings?
		false,					// Is projectile inherently inaccurate?
		false,					// Translucent colors are used?
		false,					// Good against aircraft?
	0,								// ARMING:		Time to arm projectile after launch.
	0,								// RANGE:		Inherent override range factor.
	MPH_LIGHT_SPEED,			// SPEED:		Miles per hour.
	0,								// ROT:			Rate of turn (degrees per tick).
	WARHEAD_FEEDME,			// WARHEAD:		If fires weapon, warhead type
	ANIM_NONE					// Explosion to use upon impact.
);
			
/*
**	This is the array of pointers to the static data associated with
**	each bullet (projectile) type.
*/
BulletTypeClass const** BulletTypeClass::Pointers = NULL;

BulletTypeClass const* BulletTypeClass::By_Type(BulletType type)
{
	if (Pointers != NULL) {
		return Pointers[type];
	}

	LogDebug("Initialising Bullet types");

	auto BulletCount = ReadBulletCount();

	Pointers = new BulletTypeClass const* [BulletCount] {
		&ClassSniper,		//	BULLET_SNIPER
		&ClassBullet,		//	BULLET_BULLET
		&ClassAPDS,			//	BULLET_APDS
		&Class120mm,		//	BULLET_HE
		&ClassMissile,		//	BULLET_SSM
		&ClassMissile2,	    //	BULLET_SSM2
		&ClassPatriot,		//	BULLET_SAM
		&ClassDragon,		//	BULLET_TOW
		&ClassFlame,		//  BULLET_FLAME
		&ClassChem,			//  BULLET_CHEMSPRAY
		&ClassNapalm,		//  BULLET_NAPALM
		&ClassGrenade,		//  BULLET_GRENADE
		&ClassLaser,		//  BULLET_LASER
		&ClassNukeUp,		//  BULLET_NUKE_UP
		&ClassNukeDown,	    //  BULLET_NUKE_DOWN
		&ClassHonestJohn,	//  BULLET_HONEST_JOHN
		&ClassSpreadfire,	//  BULLET_SPREADFIRE
		&ClassHeadButt,	    //  BULLET_HEADBUTT
		&ClassTRexBite,	    //  BULLET_TREXBITE
	};

	return By_Type(type);
}


/***********************************************************************************************
 * BulletTypeClass::BulletTypeClass -- Constructor for bullet type objects.                    *
 *                                                                                             *
 *    This is basically a constructor for static type objects used by bullets. All bullets     *
 *    are of a type constructed by this routine at game initialization time.                   *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
BulletTypeClass::BulletTypeClass(
	BulletType type,
	const char* rulesName,
	const char* ininame,
	const char* friendlyName,
	bool is_high,
	bool is_homing, 
	bool is_arcing, 
	bool is_dropping,
	bool is_invisible, 
	bool is_proximity_armed, 
	bool is_flame_equipped,
	bool is_fueled, 
	bool is_faceless, 
	bool is_inaccurate, 
	bool is_translucent, 
	bool is_antiaircraft,
	int arming,
	int range,
	MPHType maxspeed,
	unsigned rot,
	WarheadType warhead,
	AnimType explosion
) : ObjectTypeClass(true, false, false, true, false, false, true, true, TXT_NONE, ininame, ARMOR_NONE, 0, false) 
{
	Type = type;
	Explosion = explosion;

	RulesName = strdup(rulesName);
	IniName = strdup(ininame);
	FriendlyName = strdup(friendlyName);

	IsHigh = is_high;
	IsAntiAircraft = is_antiaircraft;
	IsTranslucent = is_translucent;
	IsArcing = is_arcing;
	IsHoming = is_homing;
	IsDropping = is_dropping;
	IsInvisible = is_invisible;
	IsProximityArmed = is_proximity_armed;
	IsFlameEquipped = is_flame_equipped;
	IsFueled = is_fueled;
	IsFaceless = is_faceless;
	IsInaccurate = is_inaccurate;

	Warhead = warhead;
	MaxSpeed = maxspeed;
	ROT = rot;
	Arming = arming;
	Range = range;

	NcoRulesRuntime().GetRules() << TiberianDawnRuleSectionBuilder::BuildBulletSection(RulesName);

	ReadBulletRules(this);
}

void BulletTypeClass::ReadBulletRules(BulletTypeClass* bullet)
{
	auto iniName = bullet->RulesName;

	ReadObjectRules(bullet, iniName);
	
	bullet->FriendlyName = NcoRulesRuntime().ReadRuleValue<char*>(iniName, FRIENDLY_NAME_RULE, bullet->FriendlyName);
	bullet->IsHigh = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_HIGH_RULE, bullet->IsHigh);
	bullet->IsAntiAircraft = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_ANTI_AIRCRAFT_RULE, bullet->IsAntiAircraft);
	bullet->IsTranslucent = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_TRANSLUCENT_RULE, bullet->IsTranslucent);
	bullet->IsArcing = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_ARCING_RULE, bullet->IsArcing);
	bullet->IsHoming = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_HOMING_RULE, bullet->IsHoming);
	bullet->IsDropping = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_DROPPING_RULE, bullet->IsDropping);
	bullet->IsInvisible = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_INVISIBLE_RULE, bullet->IsInvisible);
	bullet->IsProximityArmed = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_PROXIMITY_ARMED_RULE, bullet->IsProximityArmed);
	bullet->IsFlameEquipped = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_FLAME_EQUIPPED_RULE, bullet->IsFlameEquipped);
	bullet->IsFueled = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_FUELED_RULE, bullet->IsFueled);
	bullet->IsFaceless = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_FACELESS_RULE, bullet->IsFaceless);
	bullet->IsInaccurate = NcoRulesRuntime().ReadRuleValue<bool>(iniName, BULLET_INACCURATE_RULE, bullet->IsInaccurate);

	bullet->Warhead = NcoRulesRuntime().ReadRuleValue<WarheadType>(iniName, BULLET_WARHEAD_RULE, bullet->Warhead);
	bullet->MaxSpeed = Read_Speed(iniName, bullet->MaxSpeed);
	bullet->ROT = Read_Rate_Of_Turn(iniName, bullet->ROT);
	bullet->Arming = NcoRulesRuntime().ReadRuleValue<int>(iniName, BULLET_ARMING_RULE, bullet->Arming);
	bullet->Range = NcoRulesRuntime().ReadRuleValue<int>(iniName, BULLET_RANGE_RULE, bullet->Range);

}

void BulletTypeClass::Add_Bullet_Type(BulletTypeClass* bulletInstance)
{
	Pointers[bulletInstance->Type] = bulletInstance;
}

/***********************************************************************************************
 * BulletTypeClass::One_Time -- Performs the one time processing for bullets.                  *
 *                                                                                             *
 *    This routine is used to perform any one time processing for the bullet type class. It    *
 *    handles loading of the shape files.                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine must be called before any rendering of bullets occurs and should   *
 *             only be called once.                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void BulletTypeClass::One_Time(void)
{
	LogTrace("BulletTypeClass::One_Time start");

	BulletType index;
	char bulletCount = ReadBulletCount();
	
	LogTrace("BulletTypeClass::One_Time bulletCount %d", bulletCount);

	/*
	**	Load the bullet shapes.
	*/
	for (index = BULLET_FIRST; index < bulletCount; index++) {
		BulletTypeClass* bullet = (BulletTypeClass*)&As_Reference(index);
		char	fullname[_MAX_FNAME+_MAX_EXT];
		char* iniName = bullet->Resolve_Ini_Name();

		LogTrace("BulletTypeClass::One_Time iniName %s", iniName);

		if (!bullet->IsInvisible) {
			LogTrace("BulletTypeClass::One_Time ImageName %s", iniName);

			_makepath(fullname, NULL, NULL, iniName, ".SHP");

			RawFileClass file(fullname);

			if (file.Is_Available()) {
				LogTrace("BulletTypeClass::One_Time loading file %s", file);

				((void const *&)bullet->ImageData) = Load_Alloc_Data(file);
			} else {
				LogTrace("BulletTypeClass::One_Time retrieve from mix %s", fullname);

				((void const *&)bullet->ImageData) = MixFileClass::Retrieve(fullname);
			}

			if (bullet->ImageData == NULL) {
				iniName = bullet->Image;

				_makepath(fullname, NULL, NULL, iniName, ".SHP");

				RawFileClass file(fullname);

				if (file.Is_Available()) {
					LogTrace("BulletTypeClass::One_Time loading file %s", file);

					((void const*&)bullet->ImageData) = Load_Alloc_Data(file);
				}
				else {
					LogTrace("BulletTypeClass::One_Time retrieve from mix %s", fullname);

					((void const*&)bullet->ImageData) = MixFileClass::Retrieve(fullname);
				}
			}
		}
	}
}

char* BulletTypeClass::Resolve_Ini_Name()
{
	return IsModType
		? ModBaseIniName
		: IniName;
}