#include "function.h"

static auto USER_PROFILE_ENV_VAR = "USERPROFILE";

void Show_Error(const char* messageFormat, ...)
{
	auto logLineLength = Get_Log_Line_Length();
	auto formattedMessageBuffer = Allocate_String(logLineLength);

	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	vsnprintf(formattedMessageBuffer, logLineLength, messageFormat, formatArgs);

	va_end(formatArgs);

	Log_Error(formattedMessageBuffer);

	// we don't want a message box blocking an automated test
	#ifndef TEST_CONSOLE
	MessageBox(
		NULL,
		formattedMessageBuffer,
		"Command & Conquer",
		MB_OK | MB_ICONSTOP
	);
	#endif

	delete formattedMessageBuffer;
}

const char* Get_Env_Var_Or_Default(const char* envVarName, char* defaultValue) {
	Log_Debug("Attempting to get value of env var: %s", envVarName);

	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		Log_Warn("Env var missing or empty: %s | Returning default value: %s", envVarName, defaultValue);

		return defaultValue;
	}

	auto envVarValue = Allocate_String(requiredSize);

	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	return envVarValue;
}

char* Get_Win32_Error_Message()
{
	return Get_Win32_Error_Message(
		GetLastError()
	);
}

char* Get_Win32_Error_Message(DWORD error)
{
	LPSTR messageBuffer = nullptr;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL
	);

	return messageBuffer;
}

HANDLE Open_File_For_Appending(const char* filename, bool* errorOccured)
{
	auto file = CreateFile(
		filename,  // name of the write
		FILE_APPEND_DATA,          // open for appending
		FILE_SHARE_READ,           // share for reading only
		NULL,                      // default security
		OPEN_ALWAYS,               // open existing file or create new file 
		FILE_ATTRIBUTE_NORMAL,     // normal file
		NULL
	);

	if (file == INVALID_HANDLE_VALUE || file == NULL)
	{
		*errorOccured = true;

		return NULL;
	}

	return file;
}

void Append_To_File(HANDLE file, char* data)
{
	if (file == NULL || file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	auto dwBytesToWrite = strlen(data);

	WriteFile(
		file,              // open file handle
		data,               // start of data to write
		dwBytesToWrite,     // number of bytes to write
		NULL,    // number of bytes that were written
		NULL
	);              // no overlapped structure
}

int Parse_Number(char character)
{
	return character - '0';
}

bool Parse_Boolean(char* booleanValue)
{
	return Strings_Are_Equal(booleanValue, "TRUE");
}

bool Parse_Boolean(const char* booleanValue)
{
	auto booleanValueStr = strdup(booleanValue);

	auto result = Parse_Boolean(booleanValueStr);

	delete booleanValueStr;

	return result;
}

char* Convert_Boolean_To_String(bool booleanValue)
{
	return booleanValue ? "TRUE" : "FALSE";
}

char* Get_Mod_Data_Path()
{
	auto userProfile = Get_Env_Var_Or_Default(USER_PROFILE_ENV_VAR, NULL);

	if (String_Is_Empty(userProfile))
	{
		Show_Error("Unable to find home directory using env var '%s'\nThis should never happen...", USER_PROFILE_ENV_VAR);
		exit(1);
	}

	auto modPath = Allocate_String(MAX_PATH);

	#ifndef TEST_CONSOLE
	sprintf(modPath, "%s\\Documents\\CnCRemastered\\Mods\\Tiberian_Dawn\\nco\\Data", userProfile);
	#else
	// use the cwd for test test console app
	if (!GetCurrentDirectory(MAX_PATH, modPath))
	{
		Show_Error("Unable to lookup current working directory: %s", Get_Win32_Error_Message());
		exit(1);
	}
	#endif

	delete userProfile;

	return modPath;
}
