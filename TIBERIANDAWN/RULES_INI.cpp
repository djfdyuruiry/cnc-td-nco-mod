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
static auto LOG_LEVEL = INFO;

static bool RULES_VALID = true;
static bool LUA_IS_ENABLED = false;
static bool LUA_CONSOLE_IS_ENABLED = false;
static LuaScripts RULES_LUA_SCRIPTS;
static int GAME_TICK_INTERVAL_IN_MS;

static void Read_Lua_Scripts_From_Rules_Ini()
{
	Log_Info("Reading Lua scripts from rules ini");

	RULES_LUA_SCRIPTS = {
		NULL,
		0
	};

	bool valueFound = false;
	auto onScenarioLoadCsv = Read_Optional_String_From_Rules_Ini(NCO_RULES_SECTION_NAME, LUA_SCRIPTS_RULE, &valueFound, false);

	if (!valueFound)
	{
		return;
	}

	RULES_LUA_SCRIPTS.ScriptFiles = Parse_Csv_String(
		onScenarioLoadCsv,
		MAX_PATH,
		&RULES_LUA_SCRIPTS.ScriptFileCount
	);
}

static void Read_Log_Level_From_Rules_Ini()
{
	Log_Info("Parsing Log Level from rules ini");

	auto validLogLevels = new const char* [LOG_LEVEL_COUNT]{
		Log_Level_To_String(OFF),
		Log_Level_To_String(ERR),
		Log_Level_To_String(WARN),
		Log_Level_To_String(INFO),
		Log_Level_To_String(DEBUG),
		Log_Level_To_String(TRACE)
	};

	auto logLevelLength = Get_Log_Level_Length();
	auto logLevelBuffer = Read_String_From_Rules_Ini(
		NCO_RULES_SECTION_NAME,
		"LogLevel",
		"INFO", 
		validLogLevels,
		LOG_LEVEL_COUNT,
		false
	);

	delete validLogLevels;

	Convert_String_To_Upper_Case(logLevelBuffer);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));
}

static char* Read_Buffer_From_Rules_File(RawFileClass* rulesFile, char* rulesFilename)
{
	Log_Info("Reading rules ini from file: %s", rulesFilename);

	auto rulesBuffer = Allocate_String(rulesFile->Size());

	rulesFile->Read(rulesBuffer, rulesFile->Size());
	rulesFile->Close();

	return rulesBuffer;
}

/// <summary>
/// Load the content of the rules ini file, the location is read from the env var pointed to by
/// <ref>RULES_FILE_ENV_VAR</ref> or defaults to <ref>DEFAULT_RULES_FILENAME</ref> stored in the
/// mod path.
/// </summary>
/// <returns>The text content of the ini file or a blank string if no file was found.</returns>
static void Read_Rules_Ini_Buffers() {
	auto modPath = Get_Mod_Data_Path();

	Log_Info("Attempting to load rules ini from mod path: %s", modPath);

	auto rulesFilename = Allocate_String(MAX_PATH);
	auto defaultRulesFilename = Allocate_String(MAX_PATH);

	sprintf(rulesFilename, "%s\\%s", modPath, RULES_FILENAME);
	sprintf(defaultRulesFilename, "%s\\%s", modPath, DEFAULT_RULES_FILENAME);

	auto rulesFile = new RawFileClass(rulesFilename);
	auto defaultRulesFile = new RawFileClass(defaultRulesFilename);

	DEFAULT_RULES_INI_BUFFER = "";
	RULES_INI_BUFFER = "";

	if (!rulesFile->Is_Available()) {
		if (!defaultRulesFile->Is_Available()) {
			Log_Warn("%s not found, defaults in code will be used", DEFAULT_RULES_FILENAME);
		}

		Log_Warn("%s not found, default rules from %s be used", RULES_FILENAME, DEFAULT_RULES_FILENAME);
	}

	if (rulesFile->Is_Available())
	{
		Log_Info("Reading rules ini content: %s", rulesFilename);

		RULES_INI_BUFFER = Read_Buffer_From_Rules_File(rulesFile, rulesFilename);
	}

	if (defaultRulesFile->Is_Available())
	{
		Log_Info("Reading default rules ini content: %s", rulesFilename);

		DEFAULT_RULES_INI_BUFFER = Read_Buffer_From_Rules_File(defaultRulesFile, defaultRulesFilename);
	}

	delete modPath;
	delete rulesFilename;
	delete defaultRulesFilename;
	delete rulesFile;
	delete defaultRulesFile;
}

void Ensure_Rules_Ini_Is_Loaded() {
	if (RULES_INI_BUFFER != NULL && DEFAULT_RULES_INI_BUFFER != NULL) {
		return;
	}

	Read_Rules_Ini_Buffers();

	Read_Log_Level_From_Rules_Ini();

	LUA_IS_ENABLED = Read_Bool_From_Rules_Ini(NCO_RULES_SECTION_NAME, ENABLE_LUA_SCRIPTS_RULE, true);
	LUA_CONSOLE_IS_ENABLED = Read_Bool_From_Rules_Ini(NCO_RULES_SECTION_NAME, ENABLE_LUA_CONSOLE_RULE, false);
	GAME_TICK_INTERVAL_IN_MS = Read_Int_From_Rules_Ini(
		NCO_RULES_SECTION_NAME,
		GAME_TICK_INTERVAL_IN_MS_RULE,
		TICK_INTERVAL_IN_MILLIS,
		1,
		INT_MAX
	);

	Read_Lua_Scripts_From_Rules_Ini();
}

int Read_Optional_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound,
	bool* valueFromCache
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving optional rule value: %s -> %s", section, entry);

	bool fallbackValueFound = false;
	bool cacheHit = false;
	auto cachedValue = Get_Cached_Int_Rule(section, entry, &cacheHit);

	if (valueFound == NULL)
	{
		valueFound = &fallbackValueFound;
	}

	if (cacheHit)
	{
		if (valueFound != NULL)
		{
			*valueFound = true;
		}

		if (valueFromCache != NULL)
		{
			*valueFromCache = true;
		}

		return cachedValue;
	}

	auto value = WWGetPrivateProfileInt(
		section,
		entry,
		RULES_INI_BUFFER,
		valueFound
	);

	if (!*valueFound)
	{
		Log_Trace("No rules value found in RULES.INI buffer, reading from RULES-DEFAULT.INI buffer");

		value = WWGetPrivateProfileInt(
			section,
			entry,
			DEFAULT_RULES_INI_BUFFER,
			valueFound
		);
	}

	if (!*valueFound)
	{
		Log_Trace("No rules value found in RULES-DEFAULT.INI buffer");
	}
	else
	{
		Cache_Int_Rule(section, entry, value);
	}

	return value;
}


int Read_Optional_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
)
{
	return Read_Optional_Int_From_Rules_Ini(section, entry, valueFound, NULL);
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

	bool valueFound = false;
	bool valueFoundInCache = false;

	auto ruleValue = Read_Optional_Int_From_Rules_Ini(section, entry, &valueFound, &valueFoundInCache);

	if (valueFoundInCache)
	{
		return ruleValue;
	}

	if (!valueFound)
	{
		Log_Trace("No rules ini value found, default will be used");

		Cache_Int_Rule(section, entry, defaultValue);

		return defaultValue;
	}

	Log_Trace("Rules ini value: %d", ruleValue);

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

	if (RULES_VALID)
	{
		Cache_Int_Rule(section, entry, ruleValue);
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

int Read_Cached_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	CacheKey key,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive
)
{
	bool cacheHit = false;
	auto cacheValue = Get_Cached_Int_Rule(key, &cacheHit);

	if (cacheHit) {
		return cacheValue;
	}

	return Read_Int_From_Rules_Ini(section, entry, defaultValue, minValueInclusive, maxValueInclusive);
}

static unsigned int Read_Unsigned_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	unsigned int defaultValue,
	unsigned int minValueInclusive,
	unsigned int maxValueInclusive,
	bool* valueFound
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving rule value: %s -> %s", section, entry);
	Log_Trace("Default value: %u", defaultValue);

	bool cacheHit = false;
	auto cachedValue = Get_Cached_Double_Rule(section, entry, &cacheHit);

	if (cacheHit)
	{
		if (valueFound != NULL)
		{
			*valueFound = true;
		}

		return cachedValue;
	}

	auto ruleValueStr = Read_Optional_String_From_Rules_Ini(section, entry, valueFound, true);

	if (!*valueFound)
	{
		Log_Trace("No rules ini value found, default will be used");

		Cache_Double_Rule(section, entry, defaultValue);

		return defaultValue;
	}

	bool isValid = Is_Unsigned_Int_String(ruleValueStr);

	if (!isValid)
	{
		RULES_VALID = false;

		Show_Error(
			"Rule [%s -> %s] must be a unsigned integer number. Value provided: %s",
			section,
			entry,
			ruleValueStr
		);

		delete ruleValueStr;

		return defaultValue;
	}

	auto ruleValue = strtoul(ruleValueStr, NULL, 10);

	Log_Trace("Rules ini value: %s", ruleValueStr);

	if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
	{
		RULES_VALID = false;

		Show_Error(
			"Rule [%s -> %s] must be between %u and %u (inclusive). Value provided: %u",
			section,
			entry,
			minValueInclusive,
			maxValueInclusive,
			ruleValue
		);
	}
	else
	{
		Cache_Double_Rule(section, entry, ruleValue);
	}

	Log_Trace("Resolved value: %u", ruleValue);
	Log_Debug("Setting rule [%s -> %s] = %u", section, entry, ruleValue);

	delete ruleValueStr;

	return ruleValue;
}

unsigned int Read_Optional_Unsigned_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
)
{
	return Read_Unsigned_Int_From_Rules_Ini(
		section,
		entry,
		0u,
		0u,
		UINT_MAX,
		valueFound
	);
}

unsigned int Read_Unsigned_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	unsigned int defaultValue,
	unsigned int minValueInclusive,
	unsigned int maxValueInclusive
)
{
	bool valueFound = false;

	return Read_Unsigned_Int_From_Rules_Ini(
		section,
		entry,
		defaultValue,
		minValueInclusive,
		maxValueInclusive,
		&valueFound
	);
}

unsigned int Read_Cached_Unsigned_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	CacheKey key,
	unsigned int defaultValue,
	unsigned int minValueInclusive,
	unsigned int maxValueInclusive
)
{
	bool cacheHit = false;
	auto cacheValue = Get_Cached_Unsigned_Int_Rule(key, &cacheHit);

	if (cacheHit) {
		return cacheValue;
	}

	return Read_Unsigned_Int_From_Rules_Ini(section, entry, defaultValue, minValueInclusive, maxValueInclusive);
}

static double Read_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	double defaultValue,
	double minValueInclusive,
	double maxValueInclusive,
	bool* valueFound
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving rule value: %s -> %s", section, entry);
	Log_Trace("Default value: %f", defaultValue);

	bool cacheHit = false;
	auto cachedValue = Get_Cached_Double_Rule(section, entry, &cacheHit);

	if (cacheHit)
	{
		if (valueFound != NULL)
		{
			*valueFound = true;
		}

		return cachedValue;
	}

	auto ruleValueStr = Read_Optional_String_From_Rules_Ini(section, entry, valueFound, true);

	if (!*valueFound)
	{
		Log_Trace("No rules ini value found, default will be used");

		return defaultValue;
	}

	bool isValid = Is_Double_String(ruleValueStr);

	if (!isValid)
	{
		RULES_VALID = false;

		Show_Error(
			"Rule [%s -> %s] must be a floating point number. Value provided: %s",
			section,
			entry,
			ruleValueStr
		);

		delete ruleValueStr;

		return defaultValue;
	}

	auto ruleValue = strtod(ruleValueStr, NULL);

	Log_Trace("Rules ini value: %s", ruleValueStr);

	if (ruleValue < minValueInclusive || ruleValue > maxValueInclusive)
	{
		RULES_VALID = false;

		Show_Error(
			"Rule [%s -> %s] must be between %f and %f (inclusive). Value provided: %f",
			section,
			entry,
			minValueInclusive,
			maxValueInclusive,
			ruleValue
		);
	}
	else
	{
		Cache_Double_Rule(section, entry, ruleValue);
	}

	Log_Trace("Resolved value: %f", ruleValue);
	Log_Debug("Setting rule [%s -> %s] = %f", section, entry, ruleValue);

	delete ruleValueStr;

	return ruleValue;
}

double Read_Optional_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
)
{
	Log_Trace("Resolving optional rule value: %s -> %s", section, entry);

	return Read_Double_From_Rules_Ini(
		section,
		entry,
		0.0f,
		DBL_MIN,
		DBL_MAX,
		valueFound
	);
}

double Read_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	double defaultValue,
	double minValueInclusive,
	double maxValueInclusive
)
{
	bool valueFound = false;

	return Read_Double_From_Rules_Ini(
		section,
		entry,
		defaultValue,
		minValueInclusive,
		maxValueInclusive,
		&valueFound
	);
}

double Read_Cached_Double_From_Rules_Ini(
	const char* section,
	const char* entry,
	CacheKey key,
	double defaultValue,
	double minValueInclusive,
	double maxValueInclusive
)
{
	bool cacheHit = false;
	auto cacheValue = Get_Cached_Double_Rule(key, &cacheHit);

	if (cacheHit) {
		return cacheValue;
	}

	return Read_Double_From_Rules_Ini(section, entry, defaultValue, minValueInclusive, maxValueInclusive);
}

/**
 * Get a string without any default fallback.
 */
char* Read_Optional_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound,
	bool isForConversion
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving optional rule value: %s -> %s", section, entry);

	if (!isForConversion)
	{
		bool cacheHit = false;
		auto cachedValue = Get_Cached_String_Rule(section, entry, &cacheHit);

		if (cacheHit)
		{
			if (valueFound != NULL)
			{
				*valueFound = true;
			}

			return cachedValue;
		}
	}

	auto valueBuffer = Allocate_String(RULES_STRING_LENGTH);

	WWGetPrivateProfileString(
		section,
		entry,
		valueBuffer,
		RULES_STRING_LENGTH,
		RULES_INI_BUFFER,
		valueFound
	);

	if (!*valueFound)
	{
		Log_Trace("No rules value found in RULES.INI buffer, reading from RULES-DEFAULT.INI buffer");

		WWGetPrivateProfileString(
			section,
			entry,
			valueBuffer,
			RULES_STRING_LENGTH,
			DEFAULT_RULES_INI_BUFFER,
			valueFound
		);
	}

	if (!*valueFound)
	{
		Log_Trace("No rules value found in RULES-DEFAULT.INI buffer");
	}
	else
	{
		if (!isForConversion)
		{
			Cache_String_Rule(section, entry, valueBuffer);
		}
	}

	return valueBuffer;
}

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue,
	const char* validValues[],
	int validValueCount,
	bool isForConversion
)
{
	Ensure_Rules_Ini_Is_Loaded();

	Log_Trace("Resolving rule value: %s -> %s", section, entry);
	Log_Trace("Default value: %s", defaultValue);

	bool valueFound = false;
	auto valueBuffer = Read_Optional_String_From_Rules_Ini(section, entry, &valueFound, isForConversion);

	if (valueFound)
	{
		Log_Trace("Rules ini value: %s", valueBuffer);

		return valueBuffer;
	}
	else
	{
		Log_Trace("No rules ini value found, default will be returned");

		auto defaultCopy = strdup(defaultValue);

		Cache_String_Rule(section, entry, defaultCopy);
		
		return defaultCopy;
	}

	if (String_Is_Empty(valueBuffer))
	{
		Log_Trace("Resolved rule value was empty, default will be returned");

		auto defaultCopy = strdup(defaultValue);

		Cache_String_Rule(section, entry, defaultCopy);

		return defaultCopy;
	}

	auto valueIsValid = false;

	strupr(valueBuffer);

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
	const char* defaultValue,
	bool isForConversion
)
{
	return Read_String_From_Rules_Ini(section, entry, defaultValue, NULL, 0, isForConversion);
}


char* Read_Cached_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	CacheKey key,
	const char* defaultValue,
	bool isForConversion
)
{
	bool cacheHit = false;
	auto cacheValue = Get_Cached_String_Rule(key, &cacheHit);

	if (cacheHit) {
		return cacheValue;
	}

	return Read_String_From_Rules_Ini(section, entry, defaultValue, isForConversion);
}

bool Read_Optional_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool* valueFound
)
{
	bool cacheHit = false;
	auto cachedValue = Get_Cached_Bool_Rule(section, entry, &cacheHit);

	if (cacheHit)
	{
		if (valueFound != NULL)
		{
			*valueFound = true;
		}

		return cachedValue;
	}

	Read_Optional_String_From_Rules_Ini(
		section,
		entry,
		valueFound,
		true
	);

	if (!valueFound)
	{
		return false;
	}

	return Read_Bool_From_Rules_Ini(
		section,
		entry,
		false
	);
}

bool Read_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool defaultValue
)
{
	bool cacheHit = false;
	auto cachedValue = Get_Cached_Bool_Rule(section, entry, &cacheHit);

	if (cacheHit)
	{
		return cachedValue;
	}

	auto defaultValueStr = Convert_Boolean_To_String(defaultValue);
	auto validBoolStrings = new const char* [VALID_BOOL_STRINGS_COUNT]{ "TRUE", "FALSE" };

	auto ruleValue = Read_String_From_Rules_Ini(
		section,
		entry,
		defaultValueStr,
		validBoolStrings,
		VALID_BOOL_STRINGS_COUNT,
		true
	);

	delete validBoolStrings;

	Convert_String_To_Upper_Case(ruleValue);

	auto boolValue = Strings_Are_Equal(ruleValue, "TRUE");

	Cache_Bool_Rule(section, entry, boolValue);

	return boolValue;
}

long Read_Prerequisite(
	const char* section,
	StructType defaultValue
)
{
	auto defaultString = Structure_Type_To_String(defaultValue);
	auto structValueStr = Read_String_From_Rules_Ini(section, PREREQUISITE_RULE, defaultString, false);

	Convert_String_To_Upper_Case(structValueStr);

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

bool Read_Cached_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	CacheKey key,
	bool default
)
{
	bool cacheHit = false;
	auto cacheValue = Get_Cached_Bool_Rule(key, &cacheHit);

	if (cacheHit) {
		return cacheValue;
	}

	return Read_Bool_From_Rules_Ini(section, entry, default);
}

int Read_House_List_From_Rules_Ini(
	const char* section,
	int defaultValue,
	const char* defaultValueAsString
)
{
	bool valueFound = false;
	auto houseListCsv = Read_Optional_String_From_Rules_Ini(section, HOUSES_RULE, &valueFound, false);

	if (!valueFound)
	{
		return defaultValue;
	}

	Convert_String_To_Upper_Case(houseListCsv);

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
	auto weaponTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(weaponTypeStr);

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
	auto armorTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(armorTypeStr);

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
	auto unitSpeedTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(unitSpeedTypeStr);

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
	auto factoryTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(factoryTypeStr);

	if (Strings_Are_Equal(factoryTypeStr, defaultString))
	{
		delete factoryTypeStr;

		return defaultValue;
	}

	bool parseError = false;
	auto factoryType = Parse_Factory_Type(factoryTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a factory type
		RULES_VALID = false;

		Show_Error("Failed to parse factory type for '%s': %s", section, factoryTypeStr);

		delete factoryTypeStr;

		return FACTORY_TYPE_NONE;
	}

	delete factoryTypeStr;

	return factoryType;
}

WarheadType Read_Warhead_From_Rules_Ini(const char* section, const char* entry, WarheadType default)
{
	auto defaultString = Warhead_Type_To_String(default);
	auto warheadTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(warheadTypeStr);

	if (Strings_Are_Equal(warheadTypeStr, defaultString))
	{
		delete warheadTypeStr;

		return default;
	}

	bool parseError = false;
	auto warheadType = Parse_Warhead_Type(warheadTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a warhead type
		RULES_VALID = false;

		Show_Error("Failed to parse warhead for '%s': %s", section, warheadTypeStr);

		delete warheadTypeStr;

		return WARHEAD_NONE;
	}

	delete warheadTypeStr;

	return warheadType;
}

BulletType Read_Bullet_From_Rules_Ini(const char* section, const char* entry, BulletType default)
{
	auto defaultString = Bullet_Type_To_String(default);
	auto bulletTypeStr = Read_String_From_Rules_Ini(section, entry, defaultString, false);

	Convert_String_To_Upper_Case(bulletTypeStr);

	if (Strings_Are_Equal(bulletTypeStr, defaultString))
	{
		delete bulletTypeStr;

		return default;
	}

	bool parseError = false;
	auto bulletType = Parse_Bullet_Type(bulletTypeStr, &parseError);

	if (parseError)
	{
		// unable to parse entry as a bullet type
		RULES_VALID = false;

		Show_Error("Failed to parse bullet for '%s': %s", section, bulletTypeStr);

		delete bulletTypeStr;

		return BULLET_NONE;
	}

	delete bulletTypeStr;

	return bulletType;
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

