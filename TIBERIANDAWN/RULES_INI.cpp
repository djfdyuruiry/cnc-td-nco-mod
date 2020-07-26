#include "function.h"

#include "rules_cache.h"

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

static RulesIni* Rules;
static RulesIniReader* RuleReader;

static void Read_Lua_Scripts_From_Rules_Ini()
{
	Log_Info("Reading Lua scripts from rules ini");

	RULES_LUA_SCRIPTS = {
		NULL,
		0
	};

	auto onScenarioLoadCsv = RuleReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE);

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

	auto logLevelBuffer = RuleReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogLevel");

	Convert_String_To_Upper_Case(logLevelBuffer);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));

	LOG_PATH = RuleReader->ReadRuleValue<char*>(NCO_RULES_SECTION_NAME, "LogFile");

	if (!String_Is_Empty(LOG_PATH))
	{
		Log_Info("Resolved Log Path: %s", LOG_PATH);
	}
}

static void DefineRulesSections(RulesIni& r) {
	r << RulesIniSection::BuildSection(NCO_RULES_SECTION_NAME)
			.WithRules([](IRulesIniSection& s) {
				s << s.BuildRule("LogLevel")
						.OfType(STRING_RULE)
						.OnlyAccept(std::vector<const char*> {
							Log_Level_To_String(OFF),
							Log_Level_To_String(ERR),
							Log_Level_To_String(WARN),
							Log_Level_To_String(INFO),
							Log_Level_To_String(DEBUG),
							Log_Level_To_String(TRACE)
						})
						.WithDefault("OFF")

				  << "LogFile"                << STRING_RULE
				  << LUA_SCRIPTS_RULE         << STRING_RULE
				  << ENABLE_LUA_SCRIPTS_RULE  << BOOL_RULE
				  << ENABLE_LUA_CONSOLE_RULE  << BOOL_RULE

				  << s.BuildRule(GAME_TICK_INTERVAL_IN_MS_RULE)
						.OfType(INT_RULE)
						.WithDefault(TICK_INTERVAL_IN_MILLIS)
						.WithMin(1)
						.WithMax(INT_MAX);
			})
	  // TODO: mod rules
	  // TODO: game rules
	  << RulesIniSection::BuildSection("AI")
			.WithDefaultType(INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << ATTACK_INTERVAL_RULE
				  << BASE_SIZE_ADD_RULE
				  << POWER_SURPLUS_RULE
				  << ATTACK_DELAY_RULE
				  << POWER_EMERGENCY_FRACTION_RULE  << DOUBLE_RULE
				  << HELIPAD_RATIO_RULE             << DOUBLE_RULE
				  << HELIPAD_LIMIT_RULE
				  << AA_RATIO_RULE
				  << AA_LIMIT_RULE
				  << DEFENSE_RATIO_RULE             << DOUBLE_RULE
				  << DEFENSE_LIMIT_RULE
				  << WAR_RATIO_RULE                 << DOUBLE_RULE
				  << WAR_LIMIT_RULE
				  << BARRACKS_RATIO_RULE            << DOUBLE_RULE
				  << BARRACKS_LIMIT_RULE
				  << REFINERY_LIMIT_RULE
				  << REFINERY_RATIO_RULE            << DOUBLE_RULE
				  << AIRSTRIP_RATIO_RULE            << DOUBLE_RULE
				  << AIRSTRIP_LIMIT_RULE
				  << INFANTRY_RESERVE_RULE
				  << INFANTRY_BASE_MULT_RULE
				  << PARANOID_RULE                  << BOOL_RULE;
			})
	  << RulesIniSection::BuildSection("IQ")
			.WithDefaultType(INT_RULE)
			.WithRules([](IRulesIniSection& s) {
				s << MAX_IQ_RULE            << 5
				  << IQ_SUPER_WEAPONS_RULE
				  << IQ_PRODUCTION_RULE
				  << IQ_GUARD_AREA_RULE
				  << IQ_REPAIR_SELL_RULE
				  << IQ_CRUSH_RULE
				  << IQ_SCATTER_RULE
				  << IQ_CONTENT_SCAN_RULE
				  << IQ_AIRCRAFT_RULE
				  << IQ_HARVESTER_RULE
				  << IQ_SELL_BACK_RULE;
			});
}

void Ensure_Rules_Ini_Is_Loaded() {
	if (Rules != NULL && RuleReader != NULL) {
		return;
	}

	RulesIni& rules = RulesIni::SourceRulesFrom("RULES.INI")
		.AndThenFrom("RULES-DEFAULT.INI")
		.WithSections(&DefineRulesSections);

	RulesIniReader& ruleReader = RulesIniReader::ReaderFor(rules);

	Rules = &rules;
	RuleReader = &ruleReader;

	LUA_IS_ENABLED = RuleReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE);
	LUA_CONSOLE_IS_ENABLED = RuleReader->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE);
	GAME_TICK_INTERVAL_IN_MS = RuleReader->ReadRuleValue<int>(NCO_RULES_SECTION_NAME, GAME_TICK_INTERVAL_IN_MS_RULE);

	Read_Lua_Scripts_From_Rules_Ini();

	Read_Mods();
}

RulesIniReader& Get_Rules_Reader()
{
	Ensure_Rules_Ini_Is_Loaded();

	return *RuleReader;
}

void Rules_Ini_Failed_Validation(bool value)
{
	if (!value)
	{
		Rules->MarkAsInvalid();
	}
}

bool Rules_Ini_Failed_Validation()
{
	return !Rules->IsValid();
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
