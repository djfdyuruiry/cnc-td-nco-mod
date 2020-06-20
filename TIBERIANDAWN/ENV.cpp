#include "function.h"

/// <summary>
/// Get the value of an environment variable
/// </summary>
/// <param name="envVarName"/>
/// <param name="defaultValue"/>
/// <returns>Variable value if present and not empty, otherwise the default value provided</returns>
const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue) {
	Log_Debug("Attempting to get value of env var: %s", envVarName);

	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		Log_Warn("Env var missing or empty: %s | Returning default value: %s", envVarName, defaultValue);

		return defaultValue;
	}

	auto envVarValue = new char[requiredSize];

	// Get the value of the LIB environment variable.
	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	Log_Info("Env var value: %s", envVarValue);

	return envVarValue;
}
