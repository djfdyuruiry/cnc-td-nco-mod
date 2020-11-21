#include <shlobj.h>
#include <string>
#include <windows.h>

#include "FileUtils.h"
#include "logger.h"
#include "strings.h"
#include "utils.h"

static auto LOG_FORMAT = "%02d-%02d-%04d %02d:%02d:%02d.%03d - %s %s\n";
static const auto LOG_LINE_LENGTH = 25600u;
static const auto LOG_LEVEL_LENGTH = 5u;
static const auto LOG_TIMESTAMP_LENGTH = 20u;
static const auto LOG_SPACING_LENGTH = 4u;
static const auto LOG_FORMAT_LENGTH = LOG_TIMESTAMP_LENGTH + LOG_SPACING_LENGTH + LOG_LEVEL_LENGTH + LOG_LINE_LENGTH;

static char* CURRENT_LOG_FILE_PATH = NULL;
static LogLevel CURRENT_LOG_LEVEL = OFF;

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

LogLevel Parse_Log_Level(const char* levelString)
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

void Set_Current_Log_Path(const char* path)
{
	CURRENT_LOG_FILE_PATH = strdup(path);
}

const char* Current_Log_Path()
{
	return CURRENT_LOG_FILE_PATH;
}

void Set_Current_Log_Level(LogLevel level)
{
	CURRENT_LOG_LEVEL = level;
}

LogLevel Current_Log_Level()
{
	return CURRENT_LOG_LEVEL;
}

static void Load_Default_Log_File_Path()
{
#ifdef TEST_CONSOLE
	LOG_FILE_PATH = strdup("log\\nco.log");
#else
	auto documentsPath = Allocate_String(MAX_PATH);
	auto result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentsPath);

	if (FAILED(result))
	{
		Show_Error("Failed to read user documents path, logging to file will be disabled");
		FAILED_TO_OPEN_LOG_FILE = true;

		return;
	}

	auto cncPath = FormatString("%s\\CnCRemastered", MAX_PATH, documentsPath);

	delete documentsPath;

	if (!FileUtils::IsDirectory(cncPath))
	{
		FAILED_TO_OPEN_LOG_FILE = true;

		Log_Error("CNC Remastered document path has not been created yet, logging to file will be disabled. Path: %s", cncPath);

		delete cncPath;

		return;
	}

	LOG_FILE_PATH = FormatString("%s\\nco.log", MAX_PATH, cncPath);

	delete cncPath;
#endif	
}

static void Open_Log_File()
{
	if (LOG_FILE_HANDLE != NULL || FAILED_TO_OPEN_LOG_FILE) {
		return;
	}

	LOG_FILE_PATH = strdup(Current_Log_Path());

	if (String_Is_Empty(LOG_FILE_PATH))
	{
		Load_Default_Log_File_Path();
	}

	bool errorOccurred = false;
	LOG_FILE_HANDLE = Open_File_For_Appending(LOG_FILE_PATH, &errorOccurred);

	if (errorOccurred || LOG_FILE_HANDLE == NULL || LOG_FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		FAILED_TO_OPEN_LOG_FILE = true;

		With_Win32_Error_Message([&](auto e) {
			Show_Error("Failed to close handle for log file '%s': %s", LOG_FILE_PATH, e);
		});
	}
}

void Log(LogLevel logLevel, const char* messageFormat, ...)
{
	if (logLevel > Current_Log_Level())
	{
		return;
	}

	// format message
	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	auto formattedMessage = FormatString(messageFormat, LOG_LINE_LENGTH, formatArgs);

	va_end(formatArgs);

	// get current datetime
	SYSTEMTIME now;
	GetSystemTime(&now);

	// format final message with log level
	auto logMessage = FormatString(
		LOG_FORMAT,
		LOG_FORMAT_LENGTH,
		now.wDay,
		now.wMonth,
		now.wYear,
		now.wHour,
		now.wMinute,
		now.wSecond,
		now.wMilliseconds,
		Log_Level_To_String(logLevel),
		formattedMessage
	);

	if (!FAILED_TO_OPEN_LOG_FILE && LOG_FILE_HANDLE == NULL)
	{
		Open_Log_File();
	}

	// output to log file and console
	if (!FAILED_TO_OPEN_LOG_FILE)
	{
		Append_To_File(LOG_FILE_HANDLE, logMessage);
	}

	if (!ONLY_LOG_ERROR_TO_STD_OUT || logLevel == ERR)
	{
		printf(logMessage);
	}

	delete logMessage;
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
		if (LOG_FILE_PATH != NULL)
		{
			delete LOG_FILE_PATH;
		}

		LOG_FILE_HANDLE = NULL;
		return;
	}

	Log_Debug("Closing handle for log file: %s", LOG_FILE_PATH);

	if (!CloseHandle(LOG_FILE_HANDLE))
	{
		With_Win32_Error_Message([&](auto e) {
			Show_Error("Failed to close handle for log file '%s': %s", LOG_FILE_PATH, e);
		});
	}

	delete LOG_FILE_PATH;
	LOG_FILE_HANDLE = NULL;
}
