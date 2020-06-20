#include "function.h"
#include <conio.h>
#include <io.h>
#include "ccdde.h"

static auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
static auto DEFAULT_RULES_FILENAME = "RULES.INI";

static auto MAX_SPEED_ENTRY = "MaxSpeed";

static char* RULES_INI_BUFFER = NULL;

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
}

/// <summary>
/// Read a single value from rules ini as an integer.
/// </summary>
/// <param name="section"/>
/// <param name="entry"/>
/// <param name="defaultValue"/>
/// <returns>The value in rules ini if present, otherwise the default value provided.</returns>
int Read_Int_From_Rules_Ini(const char* section, const char* entry, int defaultValue)
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

	Log_Debug("Resolved value: %d", ruleValue);
	Log_Info("Setting rule [%s -> %s] = %d", section, entry, ruleValue);

	return ruleValue;
}

/// <summary>
/// Read the 'Strength' entry from a rules ini section as an integer.
/// </summary>
/// <param name="section"/>
/// <param name="defaultValue"/>
/// <returns>The value in rules ini section if present, otherwise the default value provided.</returns>
int Read_Strength(const char* section, int defaultValue)
{
	return Read_Int_From_Rules_Ini(section, "Strength", defaultValue);
}

/// <summary>
/// Read the 'MaxSpeed' entry from a rules ini section as an instance of <ref>MPHType</ref>. Max value: 255.
/// </summary>
/// <param name="section"/>
/// <param name="defaultValue"/>
/// <returns>The value in rules ini section if present, otherwise the default value provided.</returns>
MPHType Read_Max_Speed(const char* section, MPHType defaultValue)
{
	auto maxSpeed = Read_Int_From_Rules_Ini(section, MAX_SPEED_ENTRY, defaultValue);

	if (maxSpeed < 0 || maxSpeed > 255)
	{
		Error_And_Exit(
			"Rule [%s -> %s] must be between 0 and 255 (inclusive). Value provided: %d",
			section,
			MAX_SPEED_ENTRY,
			maxSpeed
		);
	}

	return (MPHType)maxSpeed;
}

/// <summary>
/// Read a Weapon entry from a rules ini section as an instance of <ref>WeaponType</ref>.
/// Min value: <ref>WEAPON_RIFLE</ref>
/// Max value: <ref>WEAPON_TREX</ref>
/// </summary>
/// <param name="section"/>
/// <param name="entry"/>
/// <param name="defaultValue"/>
/// <returns>The value in rules ini section if present, otherwise the default value provided.</returns>
WeaponType Read_Weapon(const char* section, const char* entry, WeaponType defaultValue)
{
	auto weapon = Read_Int_From_Rules_Ini(section, entry, defaultValue);

	if (weapon != WEAPON_NONE && (weapon < WEAPON_RIFLE || weapon > WEAPON_TREX))
	{
		Error_And_Exit(
			"Rule [%s -> %s] must be between %d and %d (inclusive). Value provided: %d",
			section,
			entry,
			WEAPON_RIFLE,
			WEAPON_TREX,
			weapon
		);
	}

	return (WeaponType)weapon;
}
