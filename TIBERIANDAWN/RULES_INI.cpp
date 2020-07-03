#include "function.h"

static auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
static auto DEFAULT_RULES_FILENAME = "RULES.INI";
static const unsigned int RULES_STRING_LENGTH = 512;

static char* RULES_INI_BUFFER = NULL;
static auto LOG_LEVEL = INFO;

static bool RULES_VALID = true;
static bool LUA_IS_ENABLED = false;
static LuaScripts RULES_LUA_SCRIPTS;

static void Read_Lua_Scripts_From_Rules_Ini()
{
	Log_Info("Reading Lua scripts from rules ini");

	RULES_LUA_SCRIPTS = {};

	auto onScenarioLoadCsv = Read_String_From_Rules_Ini(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE, "");

	RULES_LUA_SCRIPTS.ScriptFiles = Parse_Csv_String(
		onScenarioLoadCsv,
		256,
		&RULES_LUA_SCRIPTS.ScriptFileCount
	);
}

static void Read_Log_Level_From_Rules_Ini()
{
	Log_Info("Parsing Log Level from rules ini");

	auto logLevelLength = Get_Log_Level_Length();
	auto logLevelBuffer = Read_String_From_Rules_Ini(
		NCO_RULES_SECTION_NAME,
		"LogLevel",
		"INFO", 
		new const char* [6]{
			Log_Level_To_String(OFF),
			Log_Level_To_String(ERR),
			Log_Level_To_String(WARN),
			Log_Level_To_String(INFO),
			Log_Level_To_String(DEBUG),
			Log_Level_To_String(TRACE)
		},
		7
	);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));
}

/// <summary>
/// Load the content of the rules ini file, the location is read from the env var pointed to by
/// <ref>RULES_FILE_ENV_VAR</ref> or defaults to <ref>DEFAULT_RULES_FILENAME</ref> stored in the
/// mod path.
/// </summary>
/// <returns>The text content of the ini file or a blank string if no file was found.</returns>
char* Read_Rules_Ini() {
	auto modPath = Get_Mod_Data_Path();

	Log_Info("Attempting to load rules ini from mod path: %s", modPath);

	auto rulesFilename = Allocate_String(MAX_PATH);

	sprintf(rulesFilename, "%s\\%s", modPath, DEFAULT_RULES_FILENAME);

	auto rulesFile = new RawFileClass(rulesFilename);

	if (!rulesFile->Is_Available()) {
		delete rulesFile;
		delete rulesFilename;
		delete modPath;

		Log_Warn("Rules ini not found, default rules will be used");

		return "";
	}

	Log_Info("Reading rules ini from file: %s", rulesFilename);

	auto rulesBuffer = Allocate_String(rulesFile->Size());

	rulesFile->Read(rulesBuffer, rulesFile->Size());
	rulesFile->Close();

	delete rulesFile;
	delete rulesFilename;
	delete modPath;

	Log_Debug("Returning rules ini content");

	return rulesBuffer;
}

void Ensure_Rules_Ini_Is_Loaded() {
	if (RULES_INI_BUFFER != NULL) {
		return;
	}

	RULES_INI_BUFFER = Read_Rules_Ini();

	Read_Log_Level_From_Rules_Ini();

	LUA_IS_ENABLED = Read_Bool_From_Rules_Ini(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE, false);
	Read_Lua_Scripts_From_Rules_Ini();

	auto gameRulesAreValid = Init_Game_Rules();

	if (RULES_VALID)
	{
		RULES_VALID = gameRulesAreValid;
	}
}

static int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive,
	int* valueToAllowAlways
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving rule value: %s -> %s", section, entry);
	Log_Trace("Default value: %d", defaultValue);

	auto ruleValue = WWGetPrivateProfileInt(
		section,
		entry,
		defaultValue,
		RULES_INI_BUFFER
	);

	Log_Trace("WWGetPrivateProfileInt value: %d", ruleValue);

	if (valueToAllowAlways == NULL || ruleValue != *valueToAllowAlways)
	{
		if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
		{
			RULES_VALID = false;

			Show_Error(
				"Rule [%s -> %s] must be between %d and %d (inclusive). Value provided: %d",
				section,
				entry,
				minValueInclusive,
				maxValueInclusive,
				ruleValue
			);
		}
	}

	Log_Trace("Resolved value: %d", ruleValue);
	Log_Debug("Setting rule [%s -> %s] = %d", section, entry, ruleValue);

	return ruleValue;
}

int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive,
	int valueToAllowAlways
)
{
	return Read_Int_From_Rules_Ini(
		section,
		entry,
		defaultValue,
		minValueInclusive,
		maxValueInclusive,
		&valueToAllowAlways
	);
}

int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive
)
{
	return Read_Int_From_Rules_Ini(
		section,
		entry,
		defaultValue,
		minValueInclusive,
		maxValueInclusive,
		NULL
	);
}

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue,
	const char* validValues[],
	int validValueCount
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving rule value: %s -> %s", section, entry);
	Log_Trace("Default value: %s", defaultValue);

	auto valueBuffer = Allocate_String(RULES_STRING_LENGTH);

	WWGetPrivateProfileString(
		section,
		entry,
		defaultValue, 
		valueBuffer,
		RULES_STRING_LENGTH,
		RULES_INI_BUFFER
	);

	Log_Trace("WWGetPrivateProfileString value: %s", valueBuffer);

	strupr(valueBuffer);

	if (String_Is_Empty(valueBuffer))
	{
		return strdup(defaultValue);
	}

	auto valueIsValid = false;

	if (validValues == NULL || validValueCount < 1)
	{
		// no validation enabled as no values passed
		valueIsValid = true;
	}

	for (auto i = 0; i < validValueCount; i++)
	{
		if (strcmp(valueBuffer, validValues[i]) == 0)
		{
			valueIsValid = true;
			break;
		}
	}

	if (!valueIsValid)
	{
		RULES_VALID = false;

		auto validValuesCsv = Allocate_String(validValueCount * RULES_STRING_LENGTH);

		for (auto i = 0; i < validValueCount; i++)
		{
			strcat(validValuesCsv, validValues[i]);

			if (i != validValueCount - 1)
			{
				strcat(validValuesCsv, ", ");
			}
		}

		Show_Error(
			"Rule [%s -> %s] must be in the list (%s). Value provided: %s",
			section,
			entry,
			validValuesCsv,
			valueBuffer
		);

		delete validValuesCsv;
	}

	Log_Trace("Resolved value: %s", valueBuffer);
	Log_Debug("Setting rule [%s -> %s] = %s", section, entry, valueBuffer);

	return valueBuffer;
}

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue
)
{
	return Read_String_From_Rules_Ini(section, entry, defaultValue, NULL, 0);
}

bool Read_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool defaultValue
)
{
	auto defaultValueStr = Convert_Boolean_To_String(defaultValue);

	auto ruleValue = Read_String_From_Rules_Ini(
		section,
		entry,
		defaultValueStr,
		new const char* [2]{
			"TRUE",
			"FALSE"
		},
		2
	);

	return Strings_Are_Equal(ruleValue, "TRUE");
}

int Read_Prerequisite(
	const char* section,
	StructType defaultValue
)
{
	auto defaultString = Structure_Type_To_String(defaultValue);
	auto structValueStr = Read_String_From_Rules_Ini(section, PREREQUISITE_RULE, defaultString);
	bool parseError = false;
	auto structValue = Parse_Structure_Type(structValueStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a structure type
		RULES_VALID = false;

		Show_Error("Failed to parse prerequisite for '%s': %s", section, structValueStr);

		return STRUCT_NONE;
	}

	if (structValue == STRUCT_NONE)
	{
		return STRUCTF_NONE;
	}

	return 1L << structValue;
}

int Read_House_List_From_Rules_Ini(
	const char* section,
	int defaultValue,
	const char* defaultValueAsString
)
{
	auto houseListCsv = Read_String_From_Rules_Ini(section, HOUSES_RULE, defaultValueAsString);

	if (Strings_Are_Equal(houseListCsv, defaultValueAsString))
	{
		return defaultValue;
	}

	bool parseError = false;
	auto houseListBitField = Parse_House_Name_List_Csv(houseListCsv, &parseError);

	if (parseError)
	{
		RULES_VALID = false;

		Show_Error("Failed to parse houses for '%s': %s", section, houseListCsv);
	}

	return houseListBitField;
}

WeaponType Read_Weapon_Type_From_Rules_Ini(
	const char* section,
	const char* entry,
	WeaponType defaultValue
)
{
	auto defaultString = Weapon_Type_To_String(defaultValue);
	auto weaponTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString);

	if (Strings_Are_Equal(weaponTypeStr, defaultString))
	{
		return defaultValue;
	}

	bool parseError = false;
	auto weaponType = Parse_Weapon_Type(weaponTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a weapon type
		RULES_VALID = false;

		Show_Error("Failed to parse %s for '%s': %s", entry, section, weaponTypeStr);

		return WEAPON_NONE;
	}

	return weaponType;
}

ArmorType Read_Armor_Type_From_Rules_Ini(
	const char* section,
	const char* entry,
	ArmorType defaultValue
)
{
	auto defaultString = Armor_Type_To_String(defaultValue);
	auto armorTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString);

	if (Strings_Are_Equal(armorTypeStr, defaultString))
	{
		return defaultValue;
	}

	bool parseError = false;
	auto armorType = Parse_Armor_Type(armorTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a armor type
		RULES_VALID = false;
		return ARMOR_NONE;
	}

	return armorType;
}

SpeedType Read_Unit_Speed_Type_From_Rules_Ini(
	const char* section,
	const char* entry,
	SpeedType defaultValue
)
{
	auto defaultString = Unit_Speed_Type_To_String(defaultValue);
	auto unitSpeedTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString);

	if (Strings_Are_Equal(unitSpeedTypeStr, defaultString))
	{
		return defaultValue;
	}

	bool parseError = false;
	auto unitSpeedType = Parse_Unit_Speed_Type(unitSpeedTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a unit speed type
		RULES_VALID = false;

		Show_Error("Failed to parse unit speed type for '%s': %s", section, unitSpeedTypeStr);

		return SPEED_NONE;
	}

	return unitSpeedType;
}

FactoryType Read_Factory_Type_From_Rules_Ini(
	const char* section,
	const char* entry,
	FactoryType defaultValue
)
{
	auto defaultString = Factory_Type_To_String(defaultValue);
	auto factoryTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString);

	if (Strings_Are_Equal(factoryTypeStr, defaultString))
	{
		return defaultValue;
	}

	bool parseError = false;
	auto factoryType = Parse_Factory_Type(factoryTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a factory type
		RULES_VALID = false;

		Show_Error("Failed to parse factory type for '%s': %s", section, factoryTypeStr);

		return FACTORY_TYPE_NONE;
	}

	return factoryType;
}

void Rules_Ini_Failed_Validation(bool value)
{
	RULES_VALID = value;
}

bool Rules_Ini_Failed_Validation()
{
	return !RULES_VALID;
}

LogLevel Current_Log_Level()
{
	return LOG_LEVEL;
}

void Set_Current_Log_Level(LogLevel level)
{
	LOG_LEVEL = level;
}

bool Lua_Is_Enabled()
{
	Ensure_Rules_Ini_Is_Loaded();

	return LUA_IS_ENABLED;
}

LuaScripts Rules_Get_Lua_Scripts()
{
	Ensure_Rules_Ini_Is_Loaded();

	return RULES_LUA_SCRIPTS;
}

