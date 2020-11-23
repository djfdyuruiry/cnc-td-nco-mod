#include <functional>
#include <string>
#include <windows.h>

#include "Logger.h"
#include "strings.h"
#include "utils.h"

static const auto ONE_HUNDRED_NS_INTERVALS_SINCE_UNIX_EPOCH = 116444736000000000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_MS = 10000LL;
static const auto ONE_HUNDRED_NS_INTERVALS_IN_1_S = 10000000LL;
static const auto INT_MIN_STRING_LENGTH = 11u;

// length of `\TiberianDawn.xxx`
static const auto MOD_FILE_NAME_LENGTH = 17u;

static char* MOD_DATA_PATH = NULL;
static bool CONSOLE_OPEN = false;

void ShowError(const char* messageFormat, ...)
{
	if (StringIsEmpty(messageFormat))
	{
		ShowError("Message format passed to ShowError was null or blank");

		return;
	}

	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	auto formattedMessageBuffer = FormatString(messageFormat, GetLogger().GetLogLineLength, formatArgs);

	va_end(formatArgs);

	LogError(formattedMessageBuffer);

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

const char* GetEnvVar(const char* envVarName, bool* valueFound) {
	if (StringIsEmpty(envVarName))
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

	auto envVarValue = AllocateString(requiredSize);

	GetEnvironmentVariable(envVarName, envVarValue, requiredSize);

	if (valueFound != NULL)
	{
		*valueFound = true;
	}

	return envVarValue;
}

char* GetWin32ErrorMessage(DWORD error)
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

char* GetWin32ErrorMessage()
{
	return GetWin32ErrorMessage(
		GetLastError()
	);
}

void WithWin32ErrorMessage(std::function<void(const char*)> handler)
{
	auto error = GetWin32ErrorMessage();

	handler(error);

	delete error;
}

bool IsIntString(char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtol(numberValue, &remainder, 10);

	return !StringsAreEqual(numberValue, remainder);
}

bool IsIntString(const char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	auto numberValueStr = strdup(numberValue);

	auto result = IsIntString(numberValueStr);

	delete numberValueStr;

	return result;
}

bool IsUnsignedIntString(char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtoul(numberValue, &remainder, 10);

	return !StringsAreEqual(numberValue, remainder);
}

bool IsUnsignedIntString(const char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	auto numberValueStr = strdup(numberValue);

	auto result = IsUnsignedIntString(numberValueStr);

	delete numberValueStr;

	return result;
}

bool IsDoubleString(char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtod(numberValue, &remainder);

	return !StringsAreEqual(numberValue, remainder);
}

int ParseNumber(char character)
{
	return character - '0';
}

char* ConvertNumberToString(int number)
{
	auto numberBuffer = AllocateString(INT_MIN_STRING_LENGTH);

	itoa(number, numberBuffer, 10);

	return numberBuffer;
}

bool IsBooleanString(char* booleanValue)
{
	if (StringIsEmpty(booleanValue)
		|| (!StringsAreEqual(booleanValue, "TRUE") && !StringsAreEqual(booleanValue, "FALSE")))
	{
		return false;
	}

	return true;
}

bool ParseBoolean(char* booleanValue, bool* parseError)
{
	if (!IsBooleanString(booleanValue))
	{
		if (parseError != NULL)
		{
			*parseError = true;
		}

		return false;
	}

	return StringsAreEqual(booleanValue, "TRUE");
}

bool ParseBoolean(const char* booleanValue, bool* parseError)
{
	if (StringIsEmpty(booleanValue))
	{
		if (parseError != NULL)
		{
			*parseError = true;
		}

		return false;
	}

	auto booleanValueStr = strdup(booleanValue);

	auto result = ParseBoolean(booleanValueStr, parseError);

	delete booleanValueStr;

	return result;
}

bool ParseBooleanOrDefault(char* booleanValue, bool default)
{
	bool parseError = false;

	auto value = ParseBoolean(booleanValue, &parseError);

	if (parseError)
	{
		return default;
	}

	return value;
}

bool ParseBooleanOrDefault(const char* booleanValue, bool default)
{
	if (StringIsEmpty(booleanValue))
	{
		return default;
	}

	auto booleanValueStr = strdup(booleanValue);

	auto result = ParseBooleanOrDefault(booleanValueStr, default);

	delete booleanValueStr;

	return result;
}

const char* ConvertBooleanToString(bool booleanValue)
{
	return booleanValue ? "TRUE" : "FALSE";
}

long long GetNowInEpochNanos()
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

long long GetNowInEpochMillis()
{
	return GetNowInEpochNanos() / ONE_HUNDRED_NS_INTERVALS_IN_1_MS;
}

long long GetNowInEpochSecs()
{
	return GetNowInEpochNanos() / ONE_HUNDRED_NS_INTERVALS_IN_1_S;
}

const char* GetModDataPath()
{
	if (MOD_DATA_PATH != NULL)
	{
		return MOD_DATA_PATH;
	}

	HMODULE moduleHandle;

	if (
		!GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)&GetModDataPath,
			&moduleHandle
		)
		|| !Win32HandleIsValid(moduleHandle)
	)
	{
		WithWin32ErrorMessage([&](auto e) {
			ShowError("Unable to get handle on module: %s", e);
		});

		exit(1);
	}

	auto modDllPath = AllocateString(MAX_PATH);

	if (!GetModuleFileName(moduleHandle, modDllPath, MAX_PATH))
	{
		WithWin32ErrorMessage([&](auto e) {
			ShowError("Unable to get mod data path: %s", e);
		});

		exit(1);
	}

	auto modDllPathLength = strlen(modDllPath);
	auto modPathLength = modDllPathLength - MOD_FILE_NAME_LENGTH;

	MOD_DATA_PATH = AllocateString(MAX_PATH);

	for (auto c = 0u; c < modPathLength; c++)
	{
		MOD_DATA_PATH[c] = modDllPath[c];
	}

	delete modDllPath;

	LogInfo("Resolved mod data path: %s", MOD_DATA_PATH);

	return MOD_DATA_PATH;
}

char* BuildModDataFilePath(const char* filePath)
{
	return FormatString("%s\\%s", GetModDataPath(), filePath);
}

void StartConsoleOutput()
{
#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console allocation");
#else
	if (CONSOLE_OPEN)
	{
		LogWarn("Attempt to open Win32 console when it was already open detected, ignoring");
		return;
	}

	if (!AttachConsole(ATTACH_PARENT_PROCESS)
		&& !AllocConsole()
		&& !AttachConsole(GetCurrentProcessId()))
	{
		WithWin32ErrorMessage([&](auto e) {
			LogError("Error opening Win32 console: %s", e);
		});

		exit(1);
	}

	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	freopen("CON", "w", stdin);

	CONSOLE_OPEN = true;
#endif
}

void StopConsoleOutput()
{
#ifdef CI_ENV
	puts("WARNING: CI environment detected - skipped windows console stop");
#else
	if (!CONSOLE_OPEN)
	{
		LogWarn("Attempt to close Win32 console when it was not alreday open detected, ignoring");
		return;
	}

	if (!FreeConsole())
	{
		LogError("Error closing Win32 console: %s", GetWin32ErrorMessage());
	}

	fclose(stdout);
	fclose(stderr);
	fclose(stdin);

	CONSOLE_OPEN = false;
#endif
}

bool Win32HandleIsValid(HANDLE handle)
{
	return handle != NULL && handle != INVALID_HANDLE_VALUE;
}

bool CloseWin32HandleIfValid(HANDLE handle)
{
	if (!Win32HandleIsValid(handle))
	{
		return true;
	}

	return CloseHandle(handle);
}
