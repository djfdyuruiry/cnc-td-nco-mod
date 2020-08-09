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

/* $Header: /CounterStrike/RULES.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : RULES.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 05/12/96                                                     *
 *                                                                                             *
 *                  Last Update : September 10, 1996 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DifficultyClass::DifficultyClass -- Default constructor for difficulty class object.      *
 *   RulesClass::RulesClass -- Default constructor for rules class object.                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include	"function.h"


 /***********************************************************************************************
  * DifficultyClass::DifficultyClass -- Default constructor for difficulty class object.        *
  *                                                                                             *
  *    This is the default constructor for the difficulty class object. Although it initializes *
  *    the rule data with default values, it is expected that they will all be overridden by    *
  *    the rules control file.                                                                  *
  *                                                                                             *
  * INPUT:   none                                                                               *
  *                                                                                             *
  * OUTPUT:  none                                                                               *
  *                                                                                             *
  * WARNINGS:   none                                                                            *
  *                                                                                             *
  * HISTORY:                                                                                    *
  *   09/18/2019 SKY : Created.                                                                 *
  *=============================================================================================*/
DifficultyClass::DifficultyClass(void) :
	FirepowerBias(1),
	GroundspeedBias(1),
	AirspeedBias(1),
	ArmorBias(1),
	ROFBias(1),
	CostBias(1),
	BuildSpeedBias(1),
	RepairDelay(0.02f),
	BuildDelay(0.03f),
	IsBuildSlowdown(false),
	IsWallDestroyer(true),
	IsContentScan(false)
{
}


/***********************************************************************************************
 * RulesClass::RulesClass -- Default constructor for rules class object.                       *
 *                                                                                             *
 *    This is the default constructor for the rules class object. Although it initializes the  *
 *    rule data with default values, it is expected that they will all be overridden by the    *
 *    rules control file.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/17/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
RulesClass::RulesClass(void) :

	AttackInterval(Read_Unsigned_Int_Ai_Rule(ATTACK_INTERVAL_RULE)),
	BaseSizeAdd(Read_Unsigned_Int_Ai_Rule(BASE_SIZE_ADD_RULE)),
	PowerSurplus(Read_Unsigned_Int_Ai_Rule(POWER_SURPLUS_RULE)),
	AttackDelay(Read_Unsigned_Int_Ai_Rule(ATTACK_DELAY_RULE)),
	PowerEmergencyFraction(Read_Fraction_Ai_Rule(POWER_EMERGENCY_FRACTION_RULE)),
	HelipadRatio(Read_Ratio_Ai_Rule(HELIPAD_RATIO_RULE)),
	HelipadLimit(Read_Unsigned_Int_Ai_Rule(HELIPAD_LIMIT_RULE)),
	AARatio(Read_Ratio_Ai_Rule(AA_RATIO_RULE)),
	AALimit(Read_Unsigned_Int_Ai_Rule(AA_LIMIT_RULE)),
	DefenseRatio(Read_Ratio_Ai_Rule(DEFENSE_RATIO_RULE)),
	DefenseLimit(Read_Unsigned_Int_Ai_Rule(DEFENSE_LIMIT_RULE)),
	WarRatio(Read_Ratio_Ai_Rule(WAR_RATIO_RULE)),
	WarLimit(Read_Unsigned_Int_Ai_Rule(WAR_LIMIT_RULE)),
	BarracksRatio(Read_Ratio_Ai_Rule(BARRACKS_RATIO_RULE)),
	BarracksLimit(Read_Unsigned_Int_Ai_Rule(BARRACKS_LIMIT_RULE)),
	RefineryRatio(Read_Ratio_Ai_Rule(REFINERY_RATIO_RULE)),
	RefineryLimit(Read_Unsigned_Int_Ai_Rule(REFINERY_LIMIT_RULE)),
	AirstripRatio(Read_Ratio_Ai_Rule(AIRSTRIP_RATIO_RULE)),
	AirstripLimit(Read_Unsigned_Int_Ai_Rule(AIRSTRIP_LIMIT_RULE)),
	MaxIQ(Read_Max_Iq_Rule()),
	IQSuperWeapons(Read_Iq_Rule(IQ_SUPER_WEAPONS_RULE)),
	IQProduction(Read_Iq_Rule(IQ_PRODUCTION_RULE)),
	IQGuardArea(Read_Iq_Rule(IQ_GUARD_AREA_RULE)),
	IQRepairSell(Read_Iq_Rule(IQ_REPAIR_SELL_RULE)),
	IQCrush(Read_Iq_Rule(IQ_CRUSH_RULE)),
	IQScatter(Read_Iq_Rule(IQ_SCATTER_RULE)),
	IQContentScan(Read_Iq_Rule(IQ_CONTENT_SCAN_RULE)),
	IQAircraft(Read_Iq_Rule(IQ_AIRCRAFT_RULE)),
	IQHarvester(Read_Iq_Rule(IQ_HARVESTER_RULE)),
	IQSellBack(Read_Iq_Rule(IQ_SELL_BACK_RULE)),
	InfantryReserve(Read_Unsigned_Int_Ai_Rule(INFANTRY_RESERVE_RULE)),
	InfantryBaseMult(Read_Unsigned_Int_Ai_Rule(INFANTRY_BASE_MULT_RULE)),
	IsComputerParanoid(Read_Bool_Ai_Rule(PARANOID_RULE)),
	AllowSuperWeapons(true)
{
}
