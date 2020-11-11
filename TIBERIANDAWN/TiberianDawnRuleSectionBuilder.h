#pragma once

#include <rules_ini_mods.h>
#include <RulesIniSection.h>

#include "rules_ini_aircraft.h"
#include "rules_ini_building.h"
#include "rules_ini_bullet.h"
#include "rules_ini_game.h"
#include "rules_ini_generic.h"
#include "rules_ini_infantry.h"
#include "rules_ini_superweapon.h"
#include "rules_ini_unit.h"
#include "rules_ini_warhead.h"
#include "rules_ini_weapon.h"
#include "tiberian_dawn_mods.h"
#include "TiberianDawnRuleType.h"

#define DEFAULT_TRANSPORT_CAPACITY 5u

class TiberianDawnRuleSectionBuilder
{
private:
	static IRulesIniSection& BuildFriendlyNameRule(IRulesIniSection& s)
	{
		return s << FRIENDLY_NAME_RULE << STRING_RULE;
	}

	static IRulesIniSection& BuildImageRule(IRulesIniSection& s)
	{
		return s << IMAGE_RULE << STRING_RULE;
	}

	static IRulesIniSection& BuildSpeedRule(IRulesIniSection& s)
	{
		return s << s.BuildRule(SPEED_RULE)
			         .OfType(UNSIGNED_INT_RULE)
			         .WithMax(UCHAR_MAX);
	}

	static IRulesIniSection& BuildRateOfTurnRule(IRulesIniSection& s)
	{
		return s << s.BuildRule(RATE_OF_TURN_RULE)
					 .OfType(UNSIGNED_INT_RULE)
					 .WithMax(UCHAR_MAX);
	}

	static IRulesIniSection& BuildUnitAndAircraftSharedRules(IRulesIniSection& s)
	{
		return s << BuildRateOfTurnRule(s)
			     << TRANSPORTER_RULE << BOOL_RULE
			     << TRANSPORT_CAPACITY_RULE << UNSIGNED_INT_RULE;
	}

	static IRulesIniSection& BuildNonInfantrySharedRules(IRulesIniSection& s)
	{
		return s << REPAIRABLE_RULE << BOOL_RULE 
			     << HAS_CREW_RULE << BOOL_RULE
			     << TURRET_EQUIPPED_RULE << BOOL_RULE;
	}

	static IRulesIniSection& BuildGenericRules(IRulesIniSection& s)
	{
		return s << BuildFriendlyNameRule(s)
				 << BuildImageRule(s)
			     << s.BuildRule(BUILD_LEVEL_RULE)
			     	 .OfType(UNSIGNED_INT_RULE)
			     	 .WithMax(99u)
			     << s.BuildRule(SCENARIO_LEVEL_RULE)
			     	 .OfType(UNSIGNED_INT_RULE)
			     	 .WithMax(99u)
			     
			     << PREREQUISITE_RULE << PREREQ_RULE
			     << COST_RULE << UNSIGNED_INT_RULE
			     << BUILDABLE_RULE << BOOL_RULE
			     << FLAMMABLE_RULE << BOOL_RULE
			     << BuildSpeedRule(s)
			     
			     << s.BuildRule(STRENGTH_RULE)
			     	 .OfType(UNSIGNED_INT_RULE)
			     	 .WithMax(USHRT_MAX)
			     
			     << HOUSES_RULE << STRING_RULE
			     << OWNER_RULE << HOUSE_LIST_RULE
			     << PRIMARY_WEAPON_RULE << WEAPON_RULE
			     << SECONDARY_WEAPON_RULE << WEAPON_RULE
			     << ARMOR_RULE << ARMOR_TYPE_RULE
			     
			     << s.BuildRule(AMMO_RULE)
			     	 .OfType(INT_RULE)
			     	 .WithMin(-1)
			     
			     << SIGHT_RANGE_RULE << UNSIGNED_INT_RULE
			     << REWARD_RULE << UNSIGNED_INT_RULE
			     << DETECT_CLOAKED_OBJECTS_RULE << BOOL_RULE
			     << LEADER_RULE << BOOL_RULE
			     << TWO_SHOOTER_RULE << BOOL_RULE
			     << HAS_THEATER_GFX_RULE << BOOL_RULE
			     << SHOW_NAME_RULE << BOOL_RULE
			     << IS_MOD_TYPE_RULE << BOOL_RULE
			     << BASE_TYPE_RULE << STRING_RULE;
	}

public:
	static IRulesIniSection& BuildModRules()
	{
		return RulesIniSection::BuildSection(MOD_RULES_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << NEW_AIRCRAFT_RULE << STRING_RULE
				  << s.BuildRule(NEW_AIRCRAFT_COUNT_RULE)
					  .WithMax(CHAR_MAX)
				  << s.BuildRule(AIRCRAFT_COUNT_RULE)
					  .WithDefault(AIRCRAFT_COUNT)
					  .WithMin(AIRCRAFT_FIRST)
					  .WithMax(CHAR_MAX)

				  << NEW_BUILDINGS_RULE << STRING_RULE
				  << s.BuildRule(NEW_BUILDING_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(BUILDING_COUNT_RULE)
				  	  .WithDefault(STRUCT_COUNT)
				  	  .WithMin(STRUCT_FIRST)
				  	  .WithMax(CHAR_MAX)
				  
				  << NEW_BULLETS_RULE << STRING_RULE
				  << s.BuildRule(NEW_BULLET_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(BULLET_COUNT_RULE)
				  	  .WithDefault(BULLET_COUNT)
				  	  .WithMin(BULLET_FIRST)
				  	  .WithMax(CHAR_MAX)
				  
				  << NEW_INFANTRY_RULE << STRING_RULE
				  << s.BuildRule(NEW_INFANTRY_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(INFANTRY_COUNT_RULE)
				  	  .WithDefault(INFANTRY_COUNT)
				  	  .WithMin(INFANTRY_FIRST)
				  	  .WithMax(CHAR_MAX)
				  
				  << NEW_UNITS_RULE << STRING_RULE
				  << s.BuildRule(NEW_UNIT_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(UNIT_COUNT_RULE)
				  	  .WithDefault(UNIT_COUNT)
				  	  .WithMin(UNIT_FIRST)
				  	  .WithMax(CHAR_MAX)
				  
				  << NEW_WARHEADS_RULE << STRING_RULE
				  << s.BuildRule(NEW_WARHEAD_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(WARHEAD_COUNT_RULE)
				  	  .WithDefault(WARHEAD_COUNT)
				  	  .WithMin(WARHEAD_FIRST)
				  	  .WithMax(CHAR_MAX)
				  
				  << NEW_WEAPONS_RULE << STRING_RULE
				  << s.BuildRule(NEW_WEAPON_COUNT_RULE)
				  	  .WithMax(CHAR_MAX)
				  << s.BuildRule(WEAPON_COUNT_RULE)
				  	  .WithDefault(WEAPON_COUNT)
				  	  .WithMin(WEAPON_FIRST)
				  	  .WithMax(CHAR_MAX);
			});
	}

	static IRulesIniSection& BuildGameRules()
	{
		return RulesIniSection::BuildSection(GAME_RULES_SECTION_NAME)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << DEBUG_LOGGING_RULE

				  << MAX_BUILD_DISTANCE_RULE << UNSIGNED_INT_RULE << 1u
				  << PREVENT_BUILDING_IN_SHROUD_RULE << true
				  << ALLOW_BUILDING_BESIDE_WALLS_RULE << true
				  << TIBERIUM_GROWS_RULE << true
				  << TIBERIUM_SPREADS_RULE << true
				  << SLOW_TIBERIUM_RULE
				  << TIBERIUM_GROWTH_RATE_RULE << DOUBLE_RULE << 1.0
				  << TIBERIUM_SPREAD_RATE_RULE << DOUBLE_RULE << 1.0
				  << TIBERIUM_INFANTRY_DAMAGE_RULE << UNSIGNED_INT_RULE << 2u

				  << CREDITS_PER_TIBERIUM_SCOOP_RULE << UNSIGNED_INT_RULE << 25u
				  << MAX_HARVESTER_CAPACITY_RULE << UNSIGNED_INT_RULE << 28u

				  << PRODUCTION_STEPS_PER_TICK_RULE << UNSIGNED_INT_RULE << 1u
				  << FACTORY_COUNT_STEP_MULTIPLER_RULE << DOUBLE_RULE << 1.0
				  << TOTAL_PRODUCTION_STEPS_RULE << UNSIGNED_INT_RULE << 108u

				  << UNIT_REPAIR_FACTOR_RULE << DOUBLE_RULE << 1.02
				  << UNIT_REPAIR_STEP_RULE << UNSIGNED_INT_RULE << 4u
				  << AIRCRAFT_REPAIR_FACTOR_RULE << DOUBLE_RULE << 1.02
				  << AIRCRAFT_REPAIR_STEP_RULE << UNSIGNED_INT_RULE << 2u

				  << SMART_DEFENCE_RULE
				  << TARGET_TREES_RULE
				  << MCV_REDEPLOY_RULE
				  << VISCEROIDS_RULE
				  << UNITS_INDESTRUCTIBLE_RULE
				  << THREE_POINT_TURNS_RULE
				  << SHOW_BIBS_RULE
				  << NAME_CIVILIAN_BUILDINGS_RULE
				  << ONLY_ALLOW_NUKE_IF_PARTS_COLLECTED_RULE << true
				  << ONE_TIME_NUKE_RULE << true
				  << CAN_BUY_HELIPAD_ONLY_RULE
				  << SURVIVORS_FROM_BUILDINGS_RULE << true

				  << HIDE_TMPL_FROM_GDI_RULE << true
				  << HIDE_OBLI_FROM_GDI_RULE << true
				  << HIDE_APC_FROM_NOD_RULE << true
				  << HIDE_MSAM_FROM_NOD_RULE << true
				  << HIDE_HELIPAD_FROM_NOD_RULE << true
				  << HIDE_ADV_COMM_FROM_NOD_RULE << true
				  << ONLY_GDI_CAN_USE_ION_CANNON_RULE << true

				  << SET_GDI_SCENARIO_2_BUILD_LEVEL_RULE << true
				  << RENAME_TECH_CENTER_TO_PRISON_RULE << true
				  << HIDE_E3_FROM_GDI_RULE << true
				  << HIDE_MSAM_FROM_GDI_RULE << true
				  << HIDE_SBAG_FROM_GDI_RULE << true
				  << ALLOW_NOD_TO_BUILD_NUK2_EARLY_RULE << true

				  << ALLOW_BUILDING_ALL_FOR_HOUSE_RULE
				  << AUTO_SCATTER_RULE
				  << ATTACKER_ADVANTAGE_RULE
				  << SPEEDY_BUILDS_RULE;
			});
	}

	static IRulesIniSection& BuildSuperweaponRules(SectionName name, unsigned int rechargeTime)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(UNSIGNED_INT_RULE)
			<< SUPERWEAPON_RECHARGE_TIME_RULE << rechargeTime;
	}

	static IRulesIniSection& BuildWeaponSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildFriendlyNameRule(s) 
				  << WEAPON_PROJECTILE_RULE << BULLET_RULE
				  
				  << s.BuildRule(WEAPON_DAMAGE_RULE)
					  .WithMax(UCHAR_MAX)
				  << s.BuildRule(WEAPON_RATE_OF_FIRE_RULE)
					  .WithMax(UCHAR_MAX)

				  << WEAPON_RANGE_RULE
				  << BASE_TYPE_RULE << STRING_RULE
				  << IS_MOD_TYPE_RULE << BOOL_RULE;
			});
	}

	static IRulesIniSection& BuildBulletSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildFriendlyNameRule(s)
				  << BuildImageRule(s)
				  << BULLET_HIGH_RULE
				  << BULLET_ANTI_AIRCRAFT_RULE
				  << BULLET_TRANSLUCENT_RULE
				  << BULLET_ARCING_RULE
				  << BULLET_HOMING_RULE
				  << BULLET_DROPPING_RULE
				  << BULLET_INVISIBLE_RULE
				  << BULLET_PROXIMITY_ARMED_RULE
				  << BULLET_FLAME_EQUIPPED_RULE
				  << BULLET_FUELED_RULE
				  << BULLET_FACELESS_RULE
				  << BULLET_INACCURATE_RULE
				  << BULLET_WARHEAD_RULE << WARHEAD_RULE
				  << BuildSpeedRule(s)
				  << BuildRateOfTurnRule(s)
				  << BULLET_ARMING_RULE << INT_RULE
				  << BULLET_RANGE_RULE << INT_RULE
				  << IS_MOD_TYPE_RULE << BOOL_RULE
				  << BASE_TYPE_RULE << STRING_RULE;
			});
	}

	static IRulesIniSection& BuildWarheadSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildFriendlyNameRule(s)
				  << WARHEAD_SPREAD_FACTOR_RULE
				  << WARHEAD_DESTORY_WALLS_RULE << BOOL_RULE
				  << WARHEAD_DESTORY_WOOD_RULE << BOOL_RULE
				  << WARHEAD_DESTORY_TIBERIUM_RULE << BOOL_RULE
				  << WARHEAD_NO_ARMOR_MODIFIER_RULE
				  << WARHEAD_WOOD_ARMOR_MODIFIER_RULE
				  << WARHEAD_ALUMINUM_ARMOR_MODIFIER_RULE
				  << WARHEAD_STEEL_ARMOR_MODIFIER_RULE
				  << WARHEAD_CONCRETE_ARMOR_MODIFIER_RULE
				  << IS_MOD_TYPE_RULE << BOOL_RULE
  				  << BASE_TYPE_RULE << STRING_RULE;
			});
	}

	static IRulesIniSection& BuildInfantrySection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildGenericRules(s)
				  << CRUSHABLE_RULE << BOOL_RULE
				  << FEMALE_RULE
				  << CRAWLING_RULE
				  << CAN_CAPTURE_RULE
				  << FRAIDY_CAT_RULE
				  << CIVILIAN_RULE
				  << AVOIDS_TIBERIUM_RULE
				  << IMMUNE_TO_TIBERIUM_RULE
				  << HAS_C4_CHARGES_RULE;
			});
	}

	static IRulesIniSection& BuildUnitSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildGenericRules(s)
				  << BuildUnitAndAircraftSharedRules(s)
				  << BuildNonInfantrySharedRules(s)
				  << CAN_BE_FOUND_IN_CRATE_RULE
				  << LIMITED_FACINGS_RULE
				  << CAN_CRUSH_INFANTRY_RULE
				  << CAN_HARVEST_RULE
				  << RADAR_EQUIPPED_RULE
				  << HAS_FIRE_ANIMATION_RULE
				  << LOCK_TURRET_RULE
				  << HAS_TRACKS_RULE
				  << HUGE_RULE
				  << CAN_CLOAK_RULE
				  << CONSTANT_ANIMATION_RULE
				  << UNIT_SPEED_RULE << UNIT_SPEED_TYPE_RULE
				  << SELF_REPAIR_RULE;
			});
	}
	
	static IRulesIniSection& BuildAircraftSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildGenericRules(s)
				  << BuildUnitAndAircraftSharedRules(s)
				  << BuildNonInfantrySharedRules(s)
				  << CANT_HOVER_RULE
				  << CAN_LAND_RULE
				  << HAS_ROTOR_RULE;
			});
	}
	
	static IRulesIniSection& BuildBuildingSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildGenericRules(s)
				  << BuildNonInfantrySharedRules(s)
				  << BIBBED_RULE
				  << WALL_RULE
				  << FACTORY_RULE
				  << SIMPLE_DAMAGE_RULE
				  << STURDY_RULE
				  << CAPTUREABLE_RULE
				  << FIXED_SPEED_ANIMATION_RULE
				  << UNSELLABLE_RULE
				  << FACTORY_TYPE_RULE << FACTORY_RULE_TYPE
				  << STORAGE_CAPACITY_RULE << UNSIGNED_INT_RULE
				  << POWER_OUTPUT_RULE << UNSIGNED_INT_RULE
				  << POWER_INPUT_RULE << UNSIGNED_INT_RULE;
			});
	}
};
