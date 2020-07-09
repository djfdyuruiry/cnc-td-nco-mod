#ifndef RULES_INI_UNIT
#define RULES_INI_UNIT

#include "defines.h"

// unit rules
#define CAN_BE_FOUND_IN_CRATE_RULE "CanBeFoundInCrate"
#define CAN_BE_FOUND_IN_CRATE_RULE_UPPER "CANBEFOUNDINCRATE"

#define LIMITED_FACINGS_RULE "LimitedFacings"
#define LIMITED_FACINGS_RULE_UPPER "LIMITEDFACINGS"

#define CAN_CRUSH_INFANTRY_RULE "CanCrushInfantry"
#define CAN_CRUSH_INFANTRY_RULE_UPPER "CANCRUSHINFANTRY"

#define CAN_HARVEST_RULE "CanHarvest"
#define CAN_HARVEST_RULE_UPPER "CANHARVEST"

#define RADAR_EQUIPPED_RULE "RadarEquipped"
#define RADAR_EQUIPPED_RULE_UPPER "RADAREQUIPPED"

#define HAS_FIRE_ANIMATION_RULE "HasFireAnimation"
#define HAS_FIRE_ANIMATION_RULE_UPPER "HASFIREANIMATION"

#define LOCK_TURRET_RULE "LockTurret"
#define LOCK_TURRET_RULE_UPPER "LOCKTURRET"

#define HAS_TRACKS_RULE "HasTracks"
#define HAS_TRACKS_RULE_UPPER "HASTRACKS"

#define HUGE_RULE "Huge"
#define HUGE_RULE_UPPER "HUGE"

#define CAN_CLOAK_RULE "CanCloak"
#define CAN_CLOAK_RULE_UPPER "CANCLOAK"

#define CONSTANT_ANIMATION_RULE "ConstantAnimation"
#define CONSTANT_ANIMATION_RULE_UPPER "CONSTANTANIMATION"

#define UNIT_SPEED_RULE "UnitSpeed"
#define UNIT_SPEED_RULE_UPPER "UNITSPEED"

#define UNIT_RULES CAN_BE_FOUND_IN_CRATE_RULE, \
    LIMITED_FACINGS_RULE, \
    CAN_CRUSH_INFANTRY_RULE, \
    CAN_HARVEST_RULE, \
    RADAR_EQUIPPED_RULE, \
    HAS_FIRE_ANIMATION_RULE, \
    LOCK_TURRET_RULE, \
    HAS_TRACKS_RULE, \
    HUGE_RULE, \
    CAN_CLOAK_RULE, \
    CONSTANT_ANIMATION_RULE, \
    UNIT_SPEED_RULE, \
    SHARED_UNIT_AIRCRAFT_RULES, \
    SHARED_NON_INFANTRY_RULES, \
    SHARED_RULES

#define UNIT_RULE_COUNT (unsigned) 12 + SHARED_UNIT_AIRCRAFT_RULE_COUNT + SHARED_NON_INFANTRY_RULE_COUNT + SHARED_RULE_COUNT

// unit rules
#define Read_Can_Cloak(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_CLOAK_RULE, defaultValue)

#define Read_Can_Be_Found_In_Crate(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_BE_FOUND_IN_CRATE_RULE, defaultValue)

#define Read_Limited_Facings(section, defaultValue) Read_Bool_From_Rules_Ini(section, LIMITED_FACINGS_RULE, defaultValue)

#define Read_Can_Crush_Infantry(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_CRUSH_INFANTRY_RULE, defaultValue)

#define Read_Can_Harvest(section, defaultValue) Read_Bool_From_Rules_Ini(section, CAN_HARVEST_RULE, defaultValue)

#define Read_Radar_Equipped(section, defaultValue) Read_Bool_From_Rules_Ini(section, RADAR_EQUIPPED_RULE, defaultValue)

#define Read_Has_Fire_Animation(section, defaultValue) Read_Bool_From_Rules_Ini(section, HAS_FIRE_ANIMATION_RULE, defaultValue)

#define Read_Lock_Turret(section, defaultValue) Read_Bool_From_Rules_Ini(section, LOCK_TURRET_RULE, defaultValue)

#define Read_Has_Tracks(section, defaultValue) Read_Bool_From_Rules_Ini(section, HAS_TRACKS_RULE, defaultValue)

#define Read_Is_Huge(section, defaultValue) Read_Bool_From_Rules_Ini(section, HUGE_RULE, defaultValue)

#define Read_Has_Constant_Animation(section, defaultValue) Read_Bool_From_Rules_Ini(section, CONSTANT_ANIMATION_RULE, defaultValue)

#define Read_Unit_Speed(section, defaultValue) Read_Unit_Speed_Type_From_Rules_Ini(section, UNIT_SPEED_RULE, defaultValue)

SpeedType Read_Unit_Speed_Type_From_Rules_Ini(
    const char* section,
    const char* entry,
    SpeedType defaultValue
);

#endif
