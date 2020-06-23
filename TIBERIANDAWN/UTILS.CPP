#include "function.h"

void Show_Error(const char* messageFormat, ...)
{
	auto logLineLength = Get_Log_Line_Length();
	auto formattedMessageBuffer = Allocate_String(logLineLength);

	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	vsnprintf(formattedMessageBuffer, logLineLength, messageFormat, formatArgs);

	va_end(formatArgs);

	Log_Error(formattedMessageBuffer);

	MessageBox(
		NULL,
		formattedMessageBuffer,
		"Command & Conquer",
		MB_OK | MB_ICONSTOP
	);

	delete formattedMessageBuffer;
}

const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue) {
	Log_Debug("Attempting to get value of env var: %s", envVarName);

	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		Log_Warn("Env var missing or empty: %s | Returning default value: %s", envVarName, defaultValue);

		return defaultValue;
	}

	auto envVarValue = Allocate_String(requiredSize);

	// Get the value of the LIB environment variable.
	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	Log_Info("Env var value: %s", envVarValue);

	return envVarValue;
}

int Parse_Number(char character)
{
	return character - '0';
}
