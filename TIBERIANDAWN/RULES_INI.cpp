#include "function.h"

static auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
static auto DEFAULT_RULES_FILENAME = "RULES.INI";
static auto RULES_STRING_LENGTH = 16;

static char* RULES_INI_BUFFER = NULL;
static auto LOG_LEVEL = INFO;
static bool RULES_VALID = true;

char* Read_String_From_Rules_Ini(
	const char* section,
	const char* entry,
	const char* defaultValue,
	const char* validValues[],
	int validValueCount
);

static void Read_Log_Level_From_Rules_Ini()
{
	Log_Info("Parsing Log Level from rules ini");

	auto logLevelLength = Get_Log_Level_Length();
	auto logLevelBuffer = Read_String_From_Rules_Ini(
		"NCO",
		"LogLevel",
		"INFO", 
		new const char* [5]{
			Log_Level_To_String(OFF),
			Log_Level_To_String(ERR),
			Log_Level_To_String(WARN),
			Log_Level_To_String(INFO),
			Log_Level_To_String(DEBUG)
		},
		5
	);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));
}

/// <summary>
/// Load the content of the rules ini file, the location is read from the env var pointed to by
/// <ref>RULES_FILE_ENV_VAR</ref> or defaults to <ref>DEFAULT_RULES_FILENAME</ref>.
/// </summary>
/// <returns>The text content of the ini file or a blank string if no file was found.</returns>
char* Read_Rules_Ini() {
	Log_Debug("Resolving rules ini file location using env var: %s", RULES_FILE_ENV_VAR);

	auto rulesFilename = Get_Env_Var_Or_Default(RULES_FILE_ENV_VAR, DEFAULT_RULES_FILENAME);

	Log_Debug("Resolved rules file location: %s", rulesFilename);

	auto rulesFile = new RawFileClass(rulesFilename);

	if (!rulesFile->Is_Available()) {
		Log_Warn("Rules ini not found, default rules will be used");

		return "";
	}

	Log_Info("Reading rules ini from file: %s", rulesFilename);

	auto rulesBuffer = new char[rulesFile->Size()];

	rulesFile->Read(rulesBuffer, rulesFile->Size());
	rulesFile->Close();

	delete rulesFile;

	Log_Debug("Returning rules ini content");

	return rulesBuffer;
}

static void Ensure_Rules_Ini_Buffer_Is_Loaded() {
	if (RULES_INI_BUFFER != NULL) {
		return;
	}

	RULES_INI_BUFFER = Read_Rules_Ini();
	Read_Log_Level_From_Rules_Ini();
}

/// <summary>
/// Read a single value from rules ini as an integer. Resolved value
/// is validated to make sure it is in the <ref>minValueInclusive</ref> to
/// <ref>maxValueInclusive</ref> range; <ref>Show_Error_And_Exit</ref> will be
/// called if the value fails validation.
/// </summary>
/// <param name="section"/>
/// <param name="entry"/>
/// <param name="defaultValue"/>
/// <param name="minValueInclusive"/>
/// <param name="maxValueInclusive"/>
/// <param name="valueToAllowAlways">Optional pointer to the value to ignore during validation.</param>
/// <returns>The value in rules ini if present, otherwise the default value provided.</returns>
static int Read_Int_From_Rules_Ini(
	const char* section,
	const char* entry,
	int defaultValue,
	int minValueInclusive,
	int maxValueInclusive,
	int* valueToAllowAlways
)
{
	Ensure_Rules_Ini_Buffer_Is_Loaded();

	Log_Debug("Resolving rule value: %s -> %s", section, entry);
	Log_Debug("Default value: %d", defaultValue);

	auto ruleValue = WWGetPrivateProfileInt(
		section,
		entry,
		defaultValue,
		RULES_INI_BUFFER
	);

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

	Log_Debug("Resolved value: %d", ruleValue);
	Log_Debug("Setting rule [%s -> %s] = %d", section, entry, ruleValue);

	return ruleValue;
}

/// <summary>
/// Read a single value from rules ini as an integer. Resolved value
/// is validated to make sure it is in the <ref>minValueInclusive</ref> to
/// <ref>maxValueInclusive</ref> range; <ref>Show_Error_And_Exit</ref> will be
/// called if the value fails validation.
/// </summary>
/// <param name="section"/>
/// <param name="entry"/>
/// <param name="defaultValue"/>
/// <param name="minValueInclusive"/>
/// <param name="maxValueInclusive"/>
/// <param name="valueToAllowAlways">Value to ignore during validation.</param>
/// <returns>The value in rules ini if present, otherwise the default value provided.</returns>
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

/// <summary>
/// Read a single value from rules ini as an integer. Resolved value
/// is validated to make sure it is in the <ref>minValueInclusive</ref> to
/// <ref>maxValueInclusive</ref> range; <ref>Show_Error_And_Exit</ref> will be
/// called if the value fails validation.
/// </summary>
/// <param name="section"/>
/// <param name="entry"/>
/// <param name="defaultValue"/>
/// <param name="minValueInclusive"/>
/// <param name="maxValueInclusive"/>
/// <returns>The value in rules ini if present, otherwise the default value provided.</returns>
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
	Ensure_Rules_Ini_Buffer_Is_Loaded();

	Log_Debug("Resolving rule value: %s -> %s", section, entry);
	Log_Debug("Default value: %d", defaultValue);

	auto valueBuffer = new char[RULES_STRING_LENGTH];

	WWGetPrivateProfileString(
		section,
		entry,
		defaultValue, 
		valueBuffer,
		RULES_STRING_LENGTH,
		RULES_INI_BUFFER
	);

	auto valueIsValid = false;

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

		auto validValuesCsv = new char[validValueCount * RULES_STRING_LENGTH];

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

	Log_Debug("Resolved value: %s", valueBuffer);
	Log_Debug("Setting rule [%s -> %s] = %s", section, entry, valueBuffer);

	return valueBuffer;
}

bool Read_Bool_From_Rules_Ini(
	const char* section,
	const char* entry,
	bool defaultValue
)
{
	auto defaultValueStr = defaultValue ? "true" : "false";

	auto ruleValue = Read_String_From_Rules_Ini(
		section,
		entry,
		defaultValueStr,
		new const char* [2]{
			"true",
			"false"
		},
		2
	);

	return strcmp(ruleValue, "true") == 0;
}

int Read_Prerequisite(
	const char* section,
	StructType defaultValue
)
{
	auto structValue = (StructType)Read_Int_From_Rules_Ini(section, "Prerequisite", defaultValue, STRUCT_WEAP, STRUCT_WOOD_WALL, STRUCT_NONE);

	if (structValue == STRUCT_NONE)
	{
		return STRUCTF_NONE;
	}

	return 1L << structValue;
}

bool Rules_Ini_Failed_Validation()
{
	return !RULES_VALID;
}

LogLevel Get_Current_Log_Level()
{
	return LOG_LEVEL;
}
