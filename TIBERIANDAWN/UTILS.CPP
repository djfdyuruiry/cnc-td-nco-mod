#include <Shlwapi.h>

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
	#ifndef CI_ENV
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

bool Is_Boolean_String(char* booleanValue)
{
	if (String_Is_Empty(booleanValue)
		|| (!Strings_Are_Equal(booleanValue, "TRUE") && !Strings_Are_Equal(booleanValue, "FALSE")))
	{
		return false;
	}

	return true;
}

bool Parse_Boolean(char* booleanValue, bool* parseError)
{
	if (!Is_Boolean_String(booleanValue))
	{
		*parseError = true;

		return false;
	}

	return Strings_Are_Equal(booleanValue, "TRUE");
}

bool Parse_Boolean(const char* booleanValue, bool* parseError)
{
	auto booleanValueStr = strdup(booleanValue);

	auto result = Parse_Boolean(booleanValueStr, parseError);

	delete booleanValueStr;

	return result;
}

bool Parse_Boolean_Or_Default(char* booleanValue, bool default)
{
	bool parseError = false;

	auto value = Parse_Boolean(booleanValue, &parseError);

	if (parseError)
	{
		return default;
	}

	return value;
}

bool Parse_Boolean_Or_Default(const char* booleanValue, bool default)
{
	auto booleanValueStr = strdup(booleanValue);

	auto result = Parse_Boolean_Or_Default(booleanValueStr, default);

	delete booleanValueStr;

	return result;
}

char* Convert_Boolean_To_String(bool booleanValue)
{
	return booleanValue ? "TRUE" : "FALSE";
}

bool Is_Number_String(char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtol(numberValue, &remainder, 10);

	return !Strings_Are_Equal(numberValue, remainder);
}

char* Convert_Number_To_String(int number)
{
	auto numberBuffer = Allocate_String(10); // INT_MAX string length

	itoa(number, numberBuffer, 10);

	return numberBuffer;
}

char* Get_Mod_Data_Path()
{
	auto modPath = Allocate_String(MAX_PATH);

	HMODULE moduleHandle;

	if (!GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		#ifndef TEST_CONSOLE
		(LPCSTR)&NCO_Startup,
		#else
		(LPCSTR)&WinMain,
		#endif
		&moduleHandle
	))
	{
		Show_Error("Unable to get handle on module: %s", Get_Win32_Error_Message());
		exit(1);
	}

	if (!GetModuleFileName(moduleHandle, modPath, MAX_PATH))
	{
		Show_Error("Unable to get mod data path: %s", Get_Win32_Error_Message());
		exit(1);
	}

	PathRemoveFileSpec(modPath);

	return modPath;
}
