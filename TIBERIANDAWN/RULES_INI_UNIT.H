#pragma once

#include "defines.h"

#include "rules_ini.h"

// unit rules
#define CAN_BE_FOUND_IN_CRATE_RULE "CanBeFoundInCrate"

#define LIMITED_FACINGS_RULE "LimitedFacings"

#define CAN_CRUSH_INFANTRY_RULE "CanCrushInfantry"

#define CAN_HARVEST_RULE "CanHarvest"

#define RADAR_EQUIPPED_RULE "RadarEquipped"

#define HAS_FIRE_ANIMATION_RULE "HasFireAnimation"

#define LOCK_TURRET_RULE "LockTurret"

#define HAS_TRACKS_RULE "HasTracks"

#define HUGE_RULE "Huge"

#define CAN_CLOAK_RULE "CanCloak"

#define CONSTANT_ANIMATION_RULE "ConstantAnimation"

#define UNIT_SPEED_RULE "UnitSpeed"

#define SELF_REPAIR_RULE "SelfRepair"

// unit rules
#define Read_Can_Cloak(section, defaultValue) ReadRuleValue<bool>(section, CAN_CLOAK_RULE, defaultValue)

#define Read_Can_Be_Found_In_Crate(section, defaultValue) ReadRuleValue<bool>(section, CAN_BE_FOUND_IN_CRATE_RULE, defaultValue)

#define Read_Limited_Facings(section, defaultValue) ReadRuleValue<bool>(section, LIMITED_FACINGS_RULE, defaultValue)

#define Read_Can_Crush_Infantry(section, defaultValue) ReadRuleValue<bool>(section, CAN_CRUSH_INFANTRY_RULE, defaultValue)

#define Read_Can_Harvest(section, defaultValue) ReadRuleValue<bool>(section, CAN_HARVEST_RULE, defaultValue)

#define Read_Radar_Equipped(section, defaultValue) ReadRuleValue<bool>(section, RADAR_EQUIPPED_RULE, defaultValue)

#define Read_Has_Fire_Animation(section, defaultValue) ReadRuleValue<bool>(section, HAS_FIRE_ANIMATION_RULE, defaultValue)

#define Read_Lock_Turret(section, defaultValue) ReadRuleValue<bool>(section, LOCK_TURRET_RULE, defaultValue)

#define Read_Has_Tracks(section, defaultValue) ReadRuleValue<bool>(section, HAS_TRACKS_RULE, defaultValue)

#define Read_Is_Huge(section, defaultValue) ReadRuleValue<bool>(section, HUGE_RULE, defaultValue)

#define Read_Has_Constant_Animation(section, defaultValue) ReadRuleValue<bool>(section, CONSTANT_ANIMATION_RULE, defaultValue)

#define Read_Unit_Speed(section, defaultValue) ReadRuleValue<SpeedType>(section, UNIT_SPEED_RULE, defaultValue)
