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

void ShowError(const char* messageFormat, va_list formatArgs)
{
	if (StringIsEmpty(messageFormat))
	{
		ShowError("Message format passed to ShowError was null or blank");

		return;
	}

	auto formattedMessageBuffer = FormatString(messageFormat, GetLogger().GetLogLineLength(), formatArgs);

	// we don't want a message box blocking an automated test
	#ifndef CI_ENV
	MessageBox(
		NULL,
		formattedMessageBuffer,
		"NCO Mod",
		MB_OK | MB_ICONSTOP
	);
	#endif

	LogError(formattedMessageBuffer);

	delete formattedMessageBuffer;
}

void ShowError(const char* messageFormat, ...)
{
	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	ShowError(messageFormat, formatArgs);

	va_end(formatArgs);
}

void ShowErrorAndExit(const char* messageFormat, va_list formatArgs)
{
	ShowError(messageFormat, formatArgs);

	exit(1);
}

void ShowErrorAndExit(const char* messageFormat, ...)
{
	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	ShowErrorAndExit(messageFormat, formatArgs);
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

bool IsIntString(const char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtol(numberValue, &remainder, 10);

	return !StringsAreEqual(numberValue, remainder);
}

bool IsUnsignedIntString(const char* numberValue)
{
	if (StringIsEmpty(numberValue))
	{
		return false;
	}

	char* remainder;

	strtoul(numberValue, &remainder, 10);

	return !StringsAreEqual(numberValue, remainder);
}

bool IsDoubleString(const char* numberValue)
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

bool IsBooleanString(const char* booleanValue)
{
	auto booleanValueUpper = ConvertStringToUpperCase(booleanValue);

	auto isValid = !StringIsEmpty(booleanValueUpper)
		&& (StringsAreEqual(booleanValueUpper, "TRUE") || StringsAreEqual(booleanValueUpper, "FALSE"));

	delete booleanValueUpper;

	return isValid;
}

bool ParseBoolean(const char* booleanValue, bool* parseError)
{
	if (!IsBooleanString(booleanValue))
	{
		if (parseError != NULL)
		{
			*parseError = true;
		}

		return false;
	}

	auto booleanValueUpper = ConvertStringToUpperCase(booleanValue);

	auto boolValue = StringsAreEqual(booleanValueUpper, "TRUE");

	delete booleanValueUpper;

	return boolValue;
}

bool ParseBooleanOrDefault(const char* booleanValue, bool default)
{
	bool parseError = false;

	auto value = ParseBoolean(booleanValue, &parseError);

	if (parseError)
	{
		return default;
	}

	return value;
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

	MOD_DATA_PATH = AllocateString(MAX_PATH);

	HMODULE moduleHandle;

	if (
		!GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)&GetModDataPath,
			&moduleHandle
		)															  // get nco dll/exe handle...
		|| !Win32HandleIsValid(moduleHandle)						  // then check exe/dll handle is valid...
		|| !GetModuleFileName(moduleHandle, MOD_DATA_PATH, MAX_PATH)  // and finally, get path to dll/exe using handle
	)
	{
		WithWin32ErrorMessage([&](auto e) {
			ShowErrorAndExit("Unable to get NCO mod data path: %s", e);
		});
	}
	else
	{
		// remove exe/dll filename
		auto moduleDirectoryPath = ExtractSubstring(MOD_DATA_PATH, strlen(MOD_DATA_PATH) - MOD_FILE_NAME_LENGTH);

		delete MOD_DATA_PATH;

		MOD_DATA_PATH = moduleDirectoryPath;
	}

	LogInfo("Resolved NCO mod data path: %s", MOD_DATA_PATH);

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
	StopConsoleOutput();

	if (
		!AllocConsole()
		&& !AttachConsole(GetCurrentProcessId())
		&& !AttachConsole(ATTACH_PARENT_PROCESS)
	)
	{
		WithWin32ErrorMessage([&](auto e) {
			ShowError("Error opening Win32 console: %s", e);

			StopConsoleOutput();
		});

		return;
	}

	auto stdOutOpen = freopen("CON", "w", stdout);
	auto stdErrOpen = freopen("CON", "w", stderr);
	auto stdInOpen = freopen("CON", "w", stdin);

	if (stdOutOpen && stdErrOpen && stdInOpen)
	{
		return;
	}

	WithWin32ErrorMessage([&](auto e) {
		ShowError(
			"Error opening stream(s):\nSTDOUT Open: %s\nSTDERR Open: %s\nSTDERR Open: %s\n%s",
			ConvertBooleanToString(stdInOpen),
			ConvertBooleanToString(stdErrOpen),
			ConvertBooleanToString(stdOutOpen),
			e
		);

		StopConsoleOutput();
	});
#endif
}

void StopConsoleOutput()
{
#ifdef CI_ENV 
	puts("WARNING: CI environment detected - skipped windows console stop");
#else
	#ifndef TEST_CONSOLE
	if (!FreeConsole())
	{
		WithWin32ErrorMessage([&](auto e) {
			LogWarn("Error closing Win32 console: %s", e);
		});
	}

	auto stdOutClosed = fclose(stdout);
	auto stdErrClosed = fclose(stderr);
	auto stdInClosed = fclose(stdin);

	if (!stdOutClosed || !stdErrClosed || !stdInClosed)
	{
		WithWin32ErrorMessage([&](auto e) {
			ShowError(
				"Error closing stream(s):\nSTDOUT Closed: %s\nSTDERR Closed: %s\nSTDERR Closed: %s\n%s",
				ConvertBooleanToString(stdOutClosed),
				ConvertBooleanToString(stdErrClosed),
				ConvertBooleanToString(stdInClosed),
				e
			);
		});
	}
	#endif
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
