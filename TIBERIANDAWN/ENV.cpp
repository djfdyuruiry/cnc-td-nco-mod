#include <windows.h>

const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue) {
	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		return defaultValue;
	}

	auto envVarValue = (char*)malloc(requiredSize * sizeof(char));

	if (!envVarValue)
	{
		MessageBox(
			0,
			"Error - could not assign memory to read environment variable value.",
			"Command & Conquer",
			MB_OK | MB_ICONSTOP);

		exit(1);
	}

	// Get the value of the LIB environment variable.
	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	return envVarValue;
}
