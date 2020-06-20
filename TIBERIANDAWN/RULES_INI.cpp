#include "function.h"
#include <conio.h>
#include <io.h>
#include "ccdde.h"

auto RULES_FILE_ENV_VAR = "TD_RULES_FILE";
auto DEFAULT_RULES_FILENAME = "RULES.INI";

char* RULES_INI_BUFFER = NULL;

char* Read_Rules_Ini();
static void Ensure_Rules_Ini_Buffer_Is_Loaded();

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

int Read_Strength(const char* section, int defaultValue)
{
	return Read_Int_From_Rules_Ini(section, "Strength", defaultValue);
}

MPHType Read_Max_Speed(const char* section, int defaultValue)
{
	return (MPHType)Read_Int_From_Rules_Ini(section, "MaxSpeed", defaultValue);
}
