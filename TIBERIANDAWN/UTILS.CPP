#include <string>
#include <windows.h>

#include "logger.h"
#include "strings.h"

static const auto ONE_HUNDRED_NS_INTERVALS_SINCE_UNIX_EPOCH = 116444736000000000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_MS = 10000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_S = 10000000LL;

// length of `\TiberianDawn.xxx`
static const auto MOD_FILE_NAME_LENGTH = 17u;

static char* MOD_DATA_PATH = NULL;
static bool CONSOLE_OPEN = false;

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

const char* Get_Env_Var(const char* envVarName, bool* valueFound) {
	if (String_Is_Empty(envVarName))
	{
		if (valueFound != NULL)
		{
			*valueFound = false;
		}

		return NULL;
	}

	auto requiredSize = GetEnvironmentVariable(envVarName, NULL, 0);

	if (requiredSize == 0)
	{
		if (valueFound != NULL)
		{
			*valueFound = false;
		}

		return NULL;
	}

	auto envVarValue = Allocate_String(requiredSize);

	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	if (valueFound != NULL)
	{
		*valueFound = true;
	}

	return envVarValue;
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

char* Get_Win32_Error_Message()
{
	return Get_Win32_Error_Message(
		GetLastError()
	);
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

	DWORD written;

	WriteFile(
		file,
		data,
		strlen(data),
		&written,
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

bool Is_Unsigned_Int_String(char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtoul(numberValue, &remainder, 10);

	return !Strings_Are_Equal(numberValue, remainder);
}

bool Is_Unsigned_Int_String(const char* numberValue)
{
	if (String_Is_Empty(numberValue))
	{
		return false;
	}

	auto numberValueStr = strdup(numberValue);

	auto result = Is_Unsigned_Int_String(numberValueStr);

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

const char* Convert_Boolean_To_String(bool booleanValue)
{
	return booleanValue ? "TRUE" : "FALSE";
}

long long Get_Now_In_Epoch_Nanos()
{
	FILETIME now;

	GetSystemTimeAsFileTime(&now);

	LARGE_INTEGER li{};

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

const char* Get_Mod_Data_Path()
{
	if (MOD_DATA_PATH != NULL)
	{
		return MOD_DATA_PATH;
	}

	HMODULE moduleHandle;

	if (
		!GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)&Get_Mod_Data_Path,
			&moduleHandle
		)
		|| moduleHandle == NULL 
		|| moduleHandle == INVALID_HANDLE_VALUE
	)
	{
		Show_Error("Unable to get handle on module: %s", Get_Win32_Error_Message());
		exit(1);
	}

	auto modDllPath = Allocate_String(MAX_PATH);

	if (!GetModuleFileName(moduleHandle, modDllPath, MAX_PATH))
	{
		Show_Error("Unable to get mod data path: %s", Get_Win32_Error_Message());
		exit(1);
	}

	auto modDllPathLength = strlen(modDllPath);
	auto modPathLength = modDllPathLength - MOD_FILE_NAME_LENGTH;

	MOD_DATA_PATH = Allocate_String(MAX_PATH);

	for (auto c = 0u; c < modPathLength; c++)
	{
		MOD_DATA_PATH[c] = modDllPath[c];
	}

	delete modDllPath;

	Log_Info("Resolved mod data path: %s", MOD_DATA_PATH);

	return MOD_DATA_PATH;
}

char* Build_Mod_Data_File_Path(const char* filePath)
{
	auto modPath = Get_Mod_Data_Path();

	auto fullFilePath = Allocate_String(MAX_PATH);

	sprintf(fullFilePath, "%s\\%s", modPath, filePath);

	return fullFilePath;
}

void Start_Console_Output()
{
#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console allocation");
#else
	if (CONSOLE_OPEN)
	{
		Log_Warn("Attempt to open Win32 console when it was already open detected, ignoring");
		return;
	}

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

	CONSOLE_OPEN = true;
#endif
}

void Stop_Console_Output()
{
#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console stop");
#else
	if (!CONSOLE_OPEN)
	{
		Log_Warn("Attempt to close Win32 console when it was not alreday open detected, ignoring");
		return;
	}

	if (!FreeConsole())
	{
		Log_Error("Error closing Win32 console: %s", Get_Win32_Error_Message());
	}

	fclose(stdout);
	fclose(stderr);
	fclose(stdin);

	CONSOLE_OPEN = false;
#endif
}
