#ifndef RULES_CACHE_KEY_H
#define RULES_CACHE_KEY_H

const unsigned int MAX_KEY_LENGTH = 1024;

typedef unsigned long CacheKey;

CacheKey Build_Rule_Key(const char* section, const char* ruleName);

// enhancement rules
extern const CacheKey RALLY_POINTS_RULE_KEY;
extern const CacheKey MULTI_WALL_LENGTH_RULE_KEY;
extern const CacheKey FULL_COST_MULTI_WALLS_LENGTH_RULE_KEY;
extern const CacheKey COMMANDO_GUARD_RULE_KEY;

// mod rules
extern const CacheKey INFANTRY_COUNT_RULE_KEY;

// game rules
extern const CacheKey MAX_BUILD_DISTANCE_RULE_KEY;
extern const CacheKey CREDITS_PER_TIBERIUM_SCOOP_RULE_KEY;
extern const CacheKey MAX_HARVESTER_CAPACITY_RULE_KEY;
extern const CacheKey UNIT_REPAIR_STEP_RULE_KEY;
extern const CacheKey AIRCRAFT_REPAIR_STEP_RULE_KEY;

extern const CacheKey AIRCRAFT_REPAIR_FACTOR_RULE_KEY;
extern const CacheKey RENAME_TECH_CENTER_TO_PRISON_RULE_KEY;
extern const CacheKey SURVIVORS_FROM_BUILDINGS_RULE_KEY;
extern const CacheKey ALLOW_BUILDING_BESIDE_WALLS_RULE_KEY;
extern const CacheKey PREVENT_BUILDING_IN_SHROUD_RULE_KEY;
extern const CacheKey HIDE_E3_FROM_GDI_RULE_KEY;
extern const CacheKey HIDE_MSAM_FROM_GDI_RULE_KEY;
extern const CacheKey HIDE_APC_FROM_NOD_RULE_KEY;
extern const CacheKey HIDE_TMPL_FROM_GDI_RULE_KEY;
extern const CacheKey HIDE_OBLI_FROM_GDI_RULE_KEY;
extern const CacheKey HIDE_MSAM_FROM_NOD_RULE_KEY;
extern const CacheKey HIDE_ADV_COMM_FROM_NOD_RULE_KEY;
extern const CacheKey ALLOW_NOD_TO_BUILD_NUK2_EARLY_RULE_KEY;
extern const CacheKey HIDE_HELIPAD_FROM_NOD_RULE_KEY;
extern const CacheKey HIDE_SBAG_FROM_GDI_RULE_KEY;
extern const CacheKey SET_GDI_SCENARIO_2_BUILD_LEVEL_RULE_KEY;
extern const CacheKey ALLOW_BUILDING_ALL_FOR_HOUSE_RULE_KEY;
extern const CacheKey ONLY_GDI_CAN_USE_ION_CANNON_RULE_KEY;
extern const CacheKey ONE_TIME_NUKE_RULE_KEY;
extern const CacheKey ONLY_ALLOW_NUKE_IF_PARTS_COLLECTED_RULE_KEY;
extern const CacheKey TIBERIUM_INFANTRY_DAMAGE_RULE_KEY;
extern const CacheKey TIBERIUM_GROWTH_RATE_RULE_KEY;
extern const CacheKey TIBERIUM_SPREAD_RATE_RULE_KEY;
extern const CacheKey SPEEDY_BUILDS_RULE_KEY;
extern const CacheKey ATTACKER_ADVANTAGE_RULE_KEY;
extern const CacheKey DEFEND_AGAINST_FRIENDLY_FIRE_RULE_KEY;
extern const CacheKey TARGET_TREES_RULE_KEY;
extern const CacheKey MCV_REDEPLOY_RULE_KEY;
extern const CacheKey VISCEROIDS_RULE_KEY;
extern const CacheKey UNITS_INDESTRUCTIBLE_RULE_KEY;
extern const CacheKey THREE_POINT_TURNS_RULE_KEY;
extern const CacheKey TIBERIUM_GROWS_RULE_KEY;
extern const CacheKey TIBERIUM_SPREADS_RULE_KEY;
extern const CacheKey SLOW_TIBERIUM_RULE_KEY;
extern const CacheKey SHOW_BIBS_RULE_KEY;
extern const CacheKey AUTO_SCATTER_RULE_KEY;
extern const CacheKey NAME_CIVILIAN_BUILDINGS_RULE_KEY;
extern const CacheKey CAN_BUY_HELIPAD_ONLY_RULE_KEY;
extern const CacheKey UNIT_REPAIR_FACTOR_RULE_KEY;
extern const CacheKey DEBUG_LOGGING_RULE_KEY;

extern const CacheKey AI_HARVESTERS_PER_REFINERY_RULE_KEY;
extern const CacheKey HUMAN_HARVESTERS_PER_REFINERY_RULE_KEY;

#endif
