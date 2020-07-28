#include "function.h"

#include "rules_ini_nco.h"

static const auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
static const auto DEFAULT_RULES_FILENAME = "RULES-DEFAULT.INI";
static const auto RULES_FILENAME = "RULES.INI";
static const unsigned int RULES_STRING_LENGTH = MAX_PATH * 15;
static const auto VALID_BOOL_STRINGS_COUNT = 2;
static const auto ONE_SEC_IN_MILLIS = 1000;
static const auto TICK_INTERVAL_IN_MILLIS = ONE_SEC_IN_MILLIS / TICKS_PER_SECOND;

static char* RULES_INI_BUFFER = NULL;
static char* DEFAULT_RULES_INI_BUFFER = NULL;
static auto LOG_LEVEL = OFF;
static char* LOG_PATH = NULL;

static bool RULES_VALID = true;
static bool LUA_IS_ENABLED = false;
static bool LUA_CONSOLE_IS_ENABLED = false;
static LuaScripts RULES_LUA_SCRIPTS;
static int GAME_TICK_INTERVAL_IN_MS;

const char* TRUE_STRING = "TRUE";
const char* FALSE_STRING = "FALSE";

static RulesIni* RULES;
static RulesIniReader* RULE_READER;
static RulesIniInfo* RULE_INFO;

static void Read_Lua_Scripts_From_Rules_Ini()
{
	Log_Info("Reading Lua scripts from rules ini");

	RULES_LUA_SCRIPTS = {
		NULL,
		0
	};

	auto onScenarioLoadCsv = RULE_READER->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE);

	if (String_Is_Empty(onScenarioLoadCsv))
	{
		return;
	}

	RULES_LUA_SCRIPTS.ScriptFiles = Parse_Csv_String(
		onScenarioLoadCsv,
		MAX_PATH,
		&RULES_LUA_SCRIPTS.ScriptFileCount
	);
}

static void Read_Log_Settings_From_Rules_Ini()
{
	Log_Info("Parsing Log Level from rules ini");

	auto logLevelBuffer = RULE_READER->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogLevel");

	Convert_String_To_Upper_Case(logLevelBuffer);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));

	LOG_PATH = RULE_READER->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogFile");

	if (!String_Is_Empty(LOG_PATH))
	{
		Log_Info("Resolved Log Path: %s", LOG_PATH);
	}
}

static IRulesIniSection& BuildDifficultySection(
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
	bool buildSlowdown)
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

static IRulesIniSection& BuildSuperweaponSection(SectionName name, unsigned int rechargeTime)
{
	return RulesIniSection::BuildSection(name)
		.WithDefaultType(UNSIGNED_INT_RULE)
			<< SUPERWEAPON_RECHARGE_TIME_RULE << rechargeTime;
}

static void DefineRulesSections(RulesIni& r) {
	r << RulesIniSection::BuildSection(NCO_RULES_SECTION_NAME)
			.WithDefaultType(STRING_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << s.BuildRule("LogLevel")
						.OnlyAccept(std::vector<const char*> {
							Log_Level_To_String(OFF),
							Log_Level_To_String(ERR),
							Log_Level_To_String(WARN),
							Log_Level_To_String(INFO),
							Log_Level_To_String(DEBUG),
							Log_Level_To_String(TRACE)
						})
						.WithDefault("OFF")

				  << "LogFile"  
				  << LUA_SCRIPTS_RULE
				  << ENABLE_LUA_SCRIPTS_RULE  << BOOL_RULE << true
				  << ENABLE_LUA_CONSOLE_RULE  << BOOL_RULE

				  << s.BuildRule(GAME_TICK_INTERVAL_IN_MS_RULE)
						.OfType(UNSIGNED_INT_RULE)
						.WithDefault(TICK_INTERVAL_IN_MILLIS);
			})

	  << RulesIniSection::BuildSection(ENHANCEMENTS_RULES_SECTION_NAME)
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
			})
				
	  << RulesIniSection::BuildSection(MOD_RULES_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << NEW_INFANTRY_RULE << STRING_RULE
				  << s.BuildRule(NEW_INFANTRY_COUNT_RULE)
						.WithMax(CHAR_MAX)
				  << s.BuildRule(INFANTRY_COUNT_RULE)
						.WithDefault(INFANTRY_COUNT)
					    .WithMin(INFANTRY_FIRST)
						.WithMax(CHAR_MAX);
			})
				
	  << RulesIniSection::BuildSection(GAME_RULES_SECTION_NAME)
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

				  << DEFEND_AGAINST_FRIENDLY_FIRE_RULE
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
			})

	  << BuildDifficultySection(
			"Easy",
		    1.2,
		    1.2,
		    1.2,
		    0.3,
		    0.8,
		    0.8,
		    0.001,
		    0.001,
		    0.6,
		    false
		 )

	  << BuildDifficultySection(
			"Normal",
		    1.0,
		    1.0,
		    1.0,
		    1.0,
		    1.0,
		    1.0,
		    0.02,
		    0.03,
		    1.0,
		    true
		 )

	  << BuildDifficultySection(
			"Hard",
		    0.9,
		    0.9,
		    0.9,
		    1.05,
		    1.05,
		    1.0,
		    1.0,
		    0.05,
		    0.1,
			true
		 )

	  << RulesIniSection::BuildSection(AI_SECTION_NAME)
			.WithDefaultType(UNSIGNED_INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << ATTACK_INTERVAL_RULE << 3u
				  << BASE_SIZE_ADD_RULE << 3u
				  << POWER_SURPLUS_RULE << 50u
				  << ATTACK_DELAY_RULE << 1u

				  << s.BuildRule(POWER_EMERGENCY_FRACTION_RULE)
					    .OfType(FIXED_RULE)
						.WithDefault(0x00C0)
						.WithDefaultAsPercentage(0.75f)

				  << s.BuildRule(HELIPAD_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x1E)
						.WithDefaultAsPercentage(0.12f)

				  << HELIPAD_LIMIT_RULE << 5u

				  << s.BuildRule(AA_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x0024)
						.WithDefaultAsPercentage(0.14f)

				  << AA_LIMIT_RULE << 10u

				  << s.BuildRule(DEFENSE_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x0066)
						.WithDefaultAsPercentage(0.4f)

				  << DEFENSE_LIMIT_RULE << 40u

				  << s.BuildRule(WAR_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x0019)
						.WithDefaultAsPercentage(0.4f)

				  << WAR_LIMIT_RULE << 2u

				  << s.BuildRule(BARRACKS_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x0028)
						.WithDefaultAsPercentage(0.16f)

				  << BARRACKS_LIMIT_RULE << 2u
				  << REFINERY_LIMIT_RULE << 4u

				  << s.BuildRule(REFINERY_RATIO_RULE)
						.OfType(FIXED_RULE)
						.WithDefault(0x0028)
						.WithDefaultAsPercentage(0.16f)

				  << s.BuildRule(AIRSTRIP_RATIO_RULE)
					.OfType(FIXED_RULE)
						.WithDefault(0x001E)
						.WithDefaultAsPercentage(0.12f)

				  << AIRSTRIP_LIMIT_RULE << 5u
				  << INFANTRY_RESERVE_RULE << 3000u
				  << INFANTRY_BASE_MULT_RULE << 1u
				  << PARANOID_RULE << BOOL_RULE << true;
			})

	  << RulesIniSection::BuildSection(IQ_SECTION_NAME)
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
			})

	  << BuildSuperweaponSection(AIRSTRIKE_SECTION_NAME, 8u)
      << BuildSuperweaponSection(ION_CANNON_SECTION_NAME, 10u)
	  << BuildSuperweaponSection(NUCLEAR_STRIKE_SECTION_NAME, 14u);
}

void Ensure_Rules_Ini_Is_Loaded() {
	if (RULES != NULL && RULE_READER != NULL) {
		return;
	}

	RulesIni& rules = RulesIni::SourceRulesFrom("RULES.INI")
		.AndThenFrom("RULES-DEFAULT.INI")
		.WithSections(&DefineRulesSections);

	RulesIniReader& ruleReader = RulesIniReader::ReaderFor(rules);

	RULES = &rules;
	RULE_READER = &ruleReader;

	LUA_IS_ENABLED = RULE_READER->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE);
	LUA_CONSOLE_IS_ENABLED = RULE_READER->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE);
	GAME_TICK_INTERVAL_IN_MS = RULE_READER->ReadRuleValue<int>(NCO_RULES_SECTION_NAME, GAME_TICK_INTERVAL_IN_MS_RULE);

	Read_Lua_Scripts_From_Rules_Ini();

	Read_Mods();
}

RulesIni& GetRules()
{
	return *RULES;
}

RulesIniReader& GetRulesReader()
{
	Ensure_Rules_Ini_Is_Loaded();

	return *RULE_READER;
}

RulesIniInfo& GetRulesInfo()
{
	Ensure_Rules_Ini_Is_Loaded();

	return *RULE_INFO;
}

template<class T, class U> T ReadRuleValueWithSpecialDefault(SectionName section, RuleName rule, U defaultValue)
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULE_READER->ReadRuleValueWithSpecialDefault<T, U>(section, rule, defaultValue);
}

template<class T> T ReadRuleValue(SectionName section, RuleName rule, T defaultValue)
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULE_READER->ReadRuleValue<T>(section, rule, defaultValue);
}

template<class T> T ReadRuleValue(SectionName section, RuleName rule)
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULE_READER->ReadRuleValue<T>(section, rule);
}

template<class T> T ReadRuleValue(const RulesIniRuleKey key)
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULE_READER->ReadRuleValue<T>(key);
}

void MarkRulesIniAsInvalid()
{
	RULES->MarkAsInvalid();
}

bool Rules_Ini_Failed_Validation()
{
	return !RULES->IsValid();
}

LogLevel Current_Log_Level()
{
	return LOG_LEVEL;
}

void Set_Current_Log_Level(LogLevel level)
{
	LOG_LEVEL = level;
}

char* Current_Log_Path()
{
	return LOG_PATH;
}

bool Lua_Is_Enabled()
{
	Ensure_Rules_Ini_Is_Loaded();

	return LUA_IS_ENABLED;
}

bool Lua_Console_Is_Enabled()
{
	Ensure_Rules_Ini_Is_Loaded();

	return LUA_CONSOLE_IS_ENABLED;
}

LuaScripts Rules_Get_Lua_Scripts()
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULES_LUA_SCRIPTS;
}

int Rules_Get_Game_Tick_Interval_In_Ms()
{
	Ensure_Rules_Ini_Is_Loaded();

	return GAME_TICK_INTERVAL_IN_MS;
}
