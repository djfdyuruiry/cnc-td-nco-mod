#include "function.h"

const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue) {
	Log_Info("Attempting to get value of env var: %s", envVarName);

	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		Log_Info("Env var missing or empty: %s | Returning default value: %s", envVarName, defaultValue);

		return defaultValue;
	}

	auto envVarValue = (char*)malloc(requiredSize * sizeof(char));

	if (!envVarValue)
	{
		Log_Info("Env var value memory allocation failed");

		MessageBox(
			0,
			"Error - could not assign memory to read environment variable value.",
			"Command & Conquer",
			MB_OK | MB_ICONSTOP);

		exit(1);
	}

	// Get the value of the LIB environment variable.
	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	Log_Info("Env var value: %s", envVarValue);

	return envVarValue;
}
