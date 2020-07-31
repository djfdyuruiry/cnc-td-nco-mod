#pragma once

#include "./rules_ini_ai.h"
#include "./rules_ini_aircraft.h"
#include "./rules_ini_building.h"
#include "./rules_ini_bullet.h"
#include "./rules_ini_difficulty.h"
#include "./rules_ini_enhancements.h"
#include "./rules_ini_game.h"
#include "./rules_ini_generic.h"
#include "./rules_ini_infantry.h"
#include "./rules_ini_iq.h"
#include "./rules_ini_mods.h"
#include "./rules_ini_superweapon.h"
#include "./rules_ini_unit.h"
#include "./rules_ini_warhead.h"
#include "./rules_ini_weapon.h"

#define DEFAULT_TRANSPORT_CAPACITY 5u

class RulesIniTemplates
{
private:
	static IRulesIniSection& BuildFriendlyNameRule(IRulesIniSection& s)
	{
		return s << FRIENDLY_NAME_RULE << STRING_RULE;
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
		return s << HAS_CREW_RULE << BOOL_RULE
			<< TURRET_EQUIPPED_RULE << BOOL_RULE;
	}

	static IRulesIniSection& BuildGenericRules(IRulesIniSection& s)
	{
		return s << BuildFriendlyNameRule(s)
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
			<< CRUSHABLE_RULE << BOOL_RULE
			<< DETECT_CLOAKED_OBJECTS_RULE << BOOL_RULE
			<< LEADER_RULE << BOOL_RULE
			<< TWO_SHOOTER_RULE << BOOL_RULE
			<< HAS_THEATER_GFX_RULE << BOOL_RULE
			<< SHOW_NAME_RULE << BOOL_RULE
			<< IS_MOD_TYPE_RULE << BOOL_RULE
			<< REPAIRABLE_RULE << BOOL_RULE
			<< BASE_TYPE_RULE << STRING_RULE;
	}

public:
	static IRulesIniSection& BuildWeaponSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildFriendlyNameRule(s) 
				  << WEAPON_FIRES_RULE << BULLET_RULE
				  
				  << s.BuildRule(WEAPON_DAMAGE_RULE)
					  .WithMax(UCHAR_MAX)
				  << s.BuildRule(WEAPON_RATE_OF_FIRE_RULE)
					  .WithMax(UCHAR_MAX)

				  << WEAPON_RANGE_RULE
				  << IS_MOD_TYPE_RULE << BOOL_RULE;
			});
	}

	static IRulesIniSection& BuildBulletSection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildFriendlyNameRule(s)
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
				  << IS_MOD_TYPE_RULE << BOOL_RULE;
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
				  << IS_MOD_TYPE_RULE << BOOL_RULE;
			});
	}

	static IRulesIniSection& BuildInfantrySection(SectionName name)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(BOOL_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << BuildGenericRules(s)
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
