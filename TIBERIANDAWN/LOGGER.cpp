#include "function.h"

static auto LOG_FILE_PATH = "log\\nco.log";
static auto LOG_LINE_LENGTH = 1024;
static auto LOG_LEVEL_LENGTH = 5;

static HANDLE LOG_FILE_HANDLE = NULL;
static bool FAILED_TO_OPEN_LOG_FILE = false;

const char* Log_Level_To_String(LogLevel level)
{
	switch (level)
	{
		case DEBUG:
			return "DEBUG";
		case WARN:
			return "WARN";
		case ERR:
			return "ERROR";
		case INFO:
		default:
			return "INFO";
	}
}

LogLevel Parse_Log_Level(char* levelString)
{
	auto logLevel = INFO;

	if (Strings_Are_Equal(levelString, "DEBUG"))
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

	return logLevel;
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
		"%02d-%02d-%04d %02d:%02d - %s %s\n",
		now.wDay,
		now.wMonth,
		now.wYear,
		now.wHour,
		now.wMinute,
		Log_Level_To_String(logLevel),
		messageBuffer
	);

	if (!FAILED_TO_OPEN_LOG_FILE && LOG_FILE_HANDLE == NULL)
	{
		LOG_FILE_HANDLE = Open_File_For_Appending(LOG_FILE_PATH);

		if (LOG_FILE_HANDLE == NULL)
		{
			FAILED_TO_OPEN_LOG_FILE = true;
			Show_Error("Failed to open log file - check env var path is correct and/or default `log` directory is present");
		}
	}

	// output to log file and console
	if (!FAILED_TO_OPEN_LOG_FILE)
	{
		Append_To_File(LOG_FILE_HANDLE, messageWithLevelBuffer);
	}

	puts(messageWithLevelBuffer);

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

void Close_Log_File_If_Open()
{
	if (LOG_FILE_HANDLE != NULL)
	{
		Log_Debug("Closing handle for log file: %s", LOG_FILE_PATH);

		if (!CloseHandle(LOG_FILE_HANDLE))
		{
			printf("ERROR: Failed to close handle for log file: %s\n", LOG_FILE_PATH);
		}

		LOG_FILE_HANDLE = NULL;
	}
}
