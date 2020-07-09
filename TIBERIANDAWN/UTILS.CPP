#include <Shlwapi.h>

#include "function.h"

static const auto ONE_HUNDRED_NS_INTERVALS_SINCE_UNIX_EPOCH = 116444736000000000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_MS = 10000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_S = 10000000LL;

static const auto USER_PROFILE_ENV_VAR = "USERPROFILE";

void Show_Error(const char* messageFormat, ...)
{
	if (messageFormat == NULL)
	{
		Show_Error("Message format passed to Show_Error was null");

		return;
	}

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
		"NCO Mod",
		MB_OK | MB_ICONSTOP
	);
	#endif

	delete formattedMessageBuffer;
}

const char* Get_Env_Var_Or_Default(const char* envVarName, char* defaultValue) {
	if (String_Is_Empty(envVarName) && String_Is_Empty(defaultValue))
	{
		Show_Error("No env var or default passed to Get_Env_Var_Or_Default");

		return "";
	}

	if (String_Is_Empty(envVarName))
	{
		Show_Error("Env var passed to Get_Env_Var_Or_Default was null or empty");

		return defaultValue;
	}

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

	FormatMessage(
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
	if (String_Is_Empty(filename))
	{
		Show_Error("Filename passed to Open_File_For_Appending was null or empty");

		return NULL;
	}

	auto file = CreateFile(
		filename,
		FILE_APPEND_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
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
	if (file == INVALID_HANDLE_VALUE)
	{
		Show_Error("File handle passed to Append_To_File was invalid");
		return;
	}

	if (file == NULL)
	{
		Show_Error("File handle passed to Append_To_File was null");
		return;
	}

	if (String_Is_Empty(data))
	{
		return;
	}

	WriteFile(
		file,
		data,
		strlen(data),
		NULL,
		NULL
	);
}

bool Is_Int_String(char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtol(numberValue, &remainder, 10);

	return !Strings_Are_Equal(numberValue, remainder);
}

bool Is_Int_String(const char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	auto numberValueStr = strdup(numberValue);

	auto result = Is_Int_String(numberValueStr);

	delete numberValueStr;

	return result;
}

bool Is_Double_String(char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtod(numberValue, &remainder);

	return !Strings_Are_Equal(numberValue, remainder);
}

int Parse_Number(char character)
{
	return character - '0';
}

char* Convert_Number_To_String(int number)
{
	auto numberBuffer = Allocate_String(11); // INT_MIN string length

	itoa(number, numberBuffer, 10);

	return numberBuffer;
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
		if (parseError != NULL)
		{
			*parseError = true;
		}

		return false;
	}

	return Strings_Are_Equal(booleanValue, "TRUE");
}

bool Parse_Boolean(const char* booleanValue, bool* parseError)
{
	if (String_Is_Empty(booleanValue))
	{
		if (parseError != NULL)
		{
			*parseError = true;
		}

		return false;
	}

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
	if (String_Is_Empty(booleanValue))
	{
		return default;
	}

	auto booleanValueStr = strdup(booleanValue);

	auto result = Parse_Boolean_Or_Default(booleanValueStr, default);

	delete booleanValueStr;

	return result;
}

char* Convert_Boolean_To_String(bool booleanValue)
{
	return booleanValue ? "TRUE" : "FALSE";
}

char* Get_Mod_Data_Path()
{
	auto modPath = Allocate_String(MAX_PATH);

	HMODULE moduleHandle;

	if (!GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&Get_Mod_Data_Path,
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

long long Get_Now_In_Epoch_Nanos()
{
	FILETIME now;

	GetSystemTimeAsFileTime(&now);

	LARGE_INTEGER li = { 0 };

	li.LowPart = now.dwLowDateTime;
	li.HighPart = now.dwHighDateTime;

	auto nowIn100Nanos = li.QuadPart;
	auto nowIn100NanosSinceEpoch = nowIn100Nanos - ONE_HUNDRED_NS_INTERVALS_SINCE_UNIX_EPOCH;

	return nowIn100NanosSinceEpoch;
}

long long Get_Now_In_Epoch_Millis()
{
	return Get_Now_In_Epoch_Nanos() / ONE_HUNDRED_NS_INTERVALS_IN_1_MS;
}

long long Get_Now_In_Epoch_Secs()
{
	return Get_Now_In_Epoch_Nanos() / ONE_HUNDRED_NS_INTERVALS_IN_1_S;
}

void Configure_Console_Output()
{
#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console allocation");
#else
	if (!AttachConsole(ATTACH_PARENT_PROCESS)
		&& !AllocConsole()
		&& !AttachConsole(GetCurrentProcessId()))
	{
		Log_Error("Error opening Win32 console: %s", Get_Win32_Error_Message());

		exit(1);
	}

	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	freopen("CON", "w", stdin);
#endif
}
