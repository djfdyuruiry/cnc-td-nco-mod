#include "rules_ini.h"
#include "rules_ini_nco.h"
#include "RulesIniRuleKey.h"
#include "RulesIniSectionBuilder.h"

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

	  << RulesIniSectionBuilder::BuildEnhancementRules()
				
	  << RulesIniSectionBuilder::BuildModRules()
				
	  << RulesIniSectionBuilder::BuildGameRules()

	  << RulesIniSectionBuilder::BuildDifficultyRules(
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
	  << RulesIniSectionBuilder::BuildDifficultyRules(
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
	  << RulesIniSectionBuilder::BuildDifficultyRules(
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

	  << RulesIniSectionBuilder::BuildAiRules()
	  << RulesIniSectionBuilder::BuildIqRules()

	  << RulesIniSectionBuilder::BuildSuperweaponRules(AIRSTRIKE_SECTION_NAME, 8u)
      << RulesIniSectionBuilder::BuildSuperweaponRules(ION_CANNON_SECTION_NAME, 10u)
	  << RulesIniSectionBuilder::BuildSuperweaponRules(NUCLEAR_STRIKE_SECTION_NAME, 14u);
}

void Ensure_Rules_Ini_Is_Loaded() {
	if (
		RULES != NULL 
		&& RULE_READER != NULL 
		&& RULE_INFO != NULL
	) {
		return;
	}

	InitaliseRuleKeys();

	RULES = &RulesIni::SourceRulesFrom("RULES.INI")
		.AndThenFrom("RULES-DEFAULT.INI")
		.WithSections(&DefineRulesSections);

	RULE_READER = &RulesIniReader::ReaderFor(*RULES);
	RULE_INFO = &RulesIniInfo::BuildRuleInfo(*RULES);

	LUA_IS_ENABLED = 
		RULE_READER->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE);

	LUA_CONSOLE_IS_ENABLED = LUA_IS_ENABLED
		&& RULE_READER->ReadRuleValue<bool>(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE);

	GAME_TICK_INTERVAL_IN_MS = 
		RULE_READER->ReadRuleValue<int>(NCO_RULES_SECTION_NAME, GAME_TICK_INTERVAL_IN_MS_RULE);

	Read_Log_Settings_From_Rules_Ini();

	Read_Lua_Scripts_From_Rules_Ini();

	ReadMods();
}

RulesIni& GetRules()
{
	Ensure_Rules_Ini_Is_Loaded();

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

void MarkRulesIniAsInvalid()
{
	Ensure_Rules_Ini_Is_Loaded();

	RULES->MarkAsInvalid();
}

bool Rules_Ini_Failed_Validation()
{
	Ensure_Rules_Ini_Is_Loaded();

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

void Free_Rules_Memory()
{
	Log_Debug("Deleting rule objects to free memory");

	if (RULES != NULL)
	{
		delete RULES;
	}

	if (RULE_READER != NULL)
	{
		delete RULE_READER;
	}

	if (RULE_INFO != NULL)
	{
		delete RULE_INFO;
	}
}
