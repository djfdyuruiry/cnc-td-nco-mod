#include "function.h"

static auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
static auto DEFAULT_RULES_FILENAME = "RULES.INI";

static char* RULES_INI_BUFFER = NULL;
static auto LOG_LEVEL = INFO;
static bool RULES_VALID = true;

static void Read_Log_Level_From_Rules_Ini()
{
	Log_Info("Parsing Log Level from rules ini");

	auto logLevelLength = Get_Log_Level_Length();
	auto logLevelBuffer = new char[logLevelLength];

	WWGetPrivateProfileString("NCO", "LogLevel", "INFO", logLevelBuffer, logLevelLength, RULES_INI_BUFFER);

	printf("LOGGER: %s", logLevelBuffer);

	LOG_LEVEL = Parse_Log_Level(logLevelBuffer);

	Log_Info("Resolved Log Level: %s", Log_Level_To_String(LOG_LEVEL));

	delete logLevelBuffer;
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
