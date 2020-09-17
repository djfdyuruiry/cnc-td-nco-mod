#include <string>
#include <windows.h>

#include "logger.h"
#include "rules_ini.h"
#include "strings.h"
#include "utils.h"

#include <iostream>
#include <shlobj.h>

#pragma comment(lib, "shell32.lib")

static auto LOG_LINE_LENGTH = 25600;
static auto LOG_LEVEL_LENGTH = 5;

static char* LOG_FILE_PATH = NULL;
static HANDLE LOG_FILE_HANDLE = NULL;
static bool FAILED_TO_OPEN_LOG_FILE = false;
static bool ONLY_LOG_ERROR_TO_STD_OUT = false;

const char* Log_Level_To_String(LogLevel level)
{
	switch (level)
	{
		case TRACE:
			return "TRACE";
		case DEBUG:
			return "DEBUG";
		case WARN:
			return "WARN";
		case ERR:
			return "ERROR";
		case OFF:
			return "OFF";
		case INFO:
		default:
			return "INFO";
	}
}

LogLevel Parse_Log_Level(char* levelString)
{
	auto logLevel = INFO;

	if (Strings_Are_Equal(levelString, "TRACE"))
	{
		logLevel = TRACE;
	}
	else if (Strings_Are_Equal(levelString, "DEBUG"))
	{
		logLevel = DEBUG;
	}
	else if (Strings_Are_Equal(levelString, "WARN"))
	{
		logLevel = WARN;
	}
	else if (Strings_Are_Equal(levelString, "ERROR"))
	{
		logLevel = ERR;
	}
	else if (Strings_Are_Equal(levelString, "OFF"))
	{
		logLevel = OFF;
	}
	else if (!Strings_Are_Equal(levelString, "INFO"))
	{
		Show_Error("Setting log level to INFO as an unrecognised log level was provided: %s", levelString);
	}

	return logLevel;
}

LogLevel Parse_Log_Level(const char* levelString)
{
	auto levelStr = strdup(levelString);

	auto level = Parse_Log_Level(levelStr);

	delete levelStr;

	return level;
}

static void Load_Default_Log_File_Path()
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	LOG_FILE_PATH = Allocate_String(MAX_PATH);
#ifdef TEST_CONSOLE
	sprintf(LOG_FILE_PATH, "log\\nco.log");
#else
	sprintf(LOG_FILE_PATH, "%s\\CnCRemastered\\nco.log", my_documents);
#endif
}

static void Open_Log_File()
{
	if (LOG_FILE_HANDLE != NULL) {
		return;
	}
	LOG_FILE_PATH = Current_Log_Path();
	if (String_Is_Empty(LOG_FILE_PATH))
	{
		Load_Default_Log_File_Path();
	}

	if (FAILED_TO_OPEN_LOG_FILE)
	{
		return;
	}

	bool errorOccurred = false;
	LOG_FILE_HANDLE = Open_File_For_Appending(LOG_FILE_PATH, &errorOccurred);

	if (errorOccurred || LOG_FILE_HANDLE == NULL || LOG_FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		FAILED_TO_OPEN_LOG_FILE = true;

		Show_Error(
			"Failed to open log file: %s\n\nCheck folder is present and you have permission to access it: %s",
			Get_Win32_Error_Message(),
			LOG_FILE_PATH
		);
	}
}

void Log(LogLevel logLevel, const char* messageFormat, ...)
{
	if (logLevel > Current_Log_Level())
	{
		return;
	}

	auto messageBuffer = Allocate_String(LOG_LINE_LENGTH);
	auto messageWithLevelBuffer = Allocate_String(LOG_LINE_LENGTH + LOG_LEVEL_LENGTH);

	// format message using sprintf
	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	vsnprintf(messageBuffer, LOG_LINE_LENGTH, messageFormat, formatArgs);

	va_end(formatArgs);

	// get current datetime
	SYSTEMTIME now;
	GetSystemTime(&now);

	// format final message with log level
	sprintf(
		messageWithLevelBuffer,
		"%02d-%02d-%04d %02d:%02d:%02d.%03d - %s %s\n",
		now.wDay,
		now.wMonth,
		now.wYear,
		now.wHour,
		now.wMinute,
		now.wSecond,
		now.wMilliseconds,
		Log_Level_To_String(logLevel),
		messageBuffer
	);

	if (!FAILED_TO_OPEN_LOG_FILE && LOG_FILE_HANDLE == NULL)
	{
		Open_Log_File();
	}

	// output to log file and console
	if (!FAILED_TO_OPEN_LOG_FILE)
	{
		Append_To_File(LOG_FILE_HANDLE, messageWithLevelBuffer);
	}

	if (!ONLY_LOG_ERROR_TO_STD_OUT || logLevel == ERR)
	{
		printf("%s", messageWithLevelBuffer);
	}

	delete messageBuffer;
	delete messageWithLevelBuffer;
}

int Get_Log_Line_Length()
{
	return LOG_LINE_LENGTH;
}

int Get_Log_Level_Length()
{
	return LOG_LEVEL_LENGTH;
}

void Toggle_Console_Logging()
{
	ONLY_LOG_ERROR_TO_STD_OUT = !ONLY_LOG_ERROR_TO_STD_OUT;
}

bool Console_Logging_Enabled()
{
	return !ONLY_LOG_ERROR_TO_STD_OUT;
}

void Close_Log_File_If_Open()
{
	if (
		FAILED_TO_OPEN_LOG_FILE
		|| LOG_FILE_HANDLE == NULL
		|| LOG_FILE_HANDLE == INVALID_HANDLE_VALUE
	)
	{
		return;
	}

	Log_Debug("Closing handle for log file: %s", LOG_FILE_PATH);

	if (!CloseHandle(LOG_FILE_HANDLE))
	{
		Show_Error("Failed to close handle for log file '%s': %s", LOG_FILE_PATH, Get_Win32_Error_Message());
	}

	delete LOG_FILE_PATH;
	LOG_FILE_HANDLE = NULL;
}
