#pragma once

#include "rules_ini_ai.h"
#include "rules_ini_difficulty.h"
#include "rules_ini_enhancements.h"
#include "rules_ini_iq.h"
#include "rules_ini_mods.h"
#include "RulesIniSection.h"

#define DEFAULT_TRANSPORT_CAPACITY 5u

class RulesIniSectionBuilder
{
public:
	static IRulesIniSection& BuildEnhancementRules()
	{
		return RulesIniSection::BuildSection(ENHANCEMENTS_RULES_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << RALLY_POINTS_RULE << BOOL_RULE << true
				  << A_STAR_PATH_FINDING_RULE << BOOL_RULE << true
				  << COMMANDO_GUARD_RULE << BOOL_RULE << true

				  << s.BuildRule(AI_HARVESTERS_PER_REFINERY_RULE)
					  .WithDefault(1u)

				  << s.BuildRule(HUMAN_HARVESTERS_PER_REFINERY_RULE)
					  .WithDefault(1u)

				  << s.BuildRule(MULTI_WALL_LENGTH_RULE)
					  .WithDefault(1u)
					  .WithMin(1u)
					  .WithMax(10u)

				  << FULL_COST_MULTI_WALLS_LENGTH_RULE << BOOL_RULE;
			});
	}

	static IRulesIniSection& BuildDifficultyRules(
		SectionName name,
		double firepower,
		double groundspeed,
		double airspeed,
		double armor,
		double rof,
		double cost,
		double repairDelay,
		double buildDelay,
		double buildSpeed,
		bool buildSlowdown
	)
	{
		return RulesIniSection::BuildSection(name)
			.WithDefaultType(DOUBLE_RULE)
			<< FIREPOWER_DIFFICULTY_RULE << firepower
			<< GROUNDSPEED_DIFFICULTY_RULE << groundspeed
			<< AIRSPEED_DIFFICULTY_RULE << airspeed
			<< ARMOR_DIFFICULTY_RULE << armor
			<< RATE_OF_FIRE_DIFFICULTY_RULE << rof
			<< COST_DIFFICULTY_RULE << cost
			<< REPAIR_DELAY_DIFFICULTY_RULE << repairDelay
			<< BUILD_DELAY_DIFFICULTY_RULE << buildDelay
			<< BUILD_SPEED_DIFFICULTY_RULE << buildSpeed
			<< BUILD_SLOWDOWN_DIFFICULTY_RULE << BOOL_RULE << buildSlowdown;
	}

	static IRulesIniSection& BuildAiRules()
	{
		return RulesIniSection::BuildSection(AI_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << ATTACK_INTERVAL_RULE << 3u
				  << BASE_SIZE_ADD_RULE << 3u
				  << POWER_SURPLUS_RULE << 50u
				  << ATTACK_DELAY_RULE << 5u

				  << s.BuildRule(POWER_EMERGENCY_FRACTION_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x00C0)
					  .WithDefaultAsPercentage(0.75)

				  << s.BuildRule(HELIPAD_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x1E)
					  .WithDefaultAsPercentage(0.12)

				  << HELIPAD_LIMIT_RULE << 5u

				  << s.BuildRule(AA_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x0024)
					  .WithDefaultAsPercentage(0.14)

				  << AA_LIMIT_RULE << 10u

				  << s.BuildRule(DEFENSE_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x0066)
					  .WithDefaultAsPercentage(0.4)

				  << DEFENSE_LIMIT_RULE << 40u

				  << s.BuildRule(WAR_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x0019)
					  .WithDefaultAsPercentage(0.1)

				  << WAR_LIMIT_RULE << 2u

				  << s.BuildRule(BARRACKS_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x0028)
					  .WithDefaultAsPercentage(0.16)

				  << BARRACKS_LIMIT_RULE << 2u
				  << REFINERY_LIMIT_RULE << 4u

				  << s.BuildRule(REFINERY_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x0028)
					  .WithDefaultAsPercentage(0.16)

				  << s.BuildRule(AIRSTRIP_RATIO_RULE)
					  .OfType(FIXED_RULE)
					  .WithDefault(0x001E)
					  .WithDefaultAsPercentage(0.12)

				  << AIRSTRIP_LIMIT_RULE << 5u
				  << INFANTRY_RESERVE_RULE << 3000u
				  << INFANTRY_BASE_MULT_RULE << 1u
				  << PARANOID_RULE << BOOL_RULE << true;
			});
	}

	static IRulesIniSection& BuildIqRules()
	{
		return RulesIniSection::BuildSection(IQ_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << MAX_IQ_RULE << 5
				  << IQ_SUPER_WEAPONS_RULE << 4u
				  << IQ_PRODUCTION_RULE << 5u
				  << IQ_GUARD_AREA_RULE << 4u
				  << IQ_REPAIR_SELL_RULE << 1u
				  << IQ_CRUSH_RULE << 2u
				  << IQ_SCATTER_RULE << 3u
				  << IQ_CONTENT_SCAN_RULE << 4u
				  << IQ_AIRCRAFT_RULE << 4u
				  << IQ_HARVESTER_RULE << 2u
				  << IQ_SELL_BACK_RULE << 2u;
			});
	}
};
