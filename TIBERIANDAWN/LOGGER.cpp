#include "function.h"

static auto LOG_FILE_PATH = "log\\nco.log";
static auto LOG_LINE_LENGTH = 1024;
static auto LOG_LEVEL_LENGTH = 5;

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

/// <summary>
/// Log a message to the file pointed to by <ref>LOG_FILE_PATH</ref>.
/// </summary>
/// <param name="logLevel"/>
/// <param name="messageFormat">
///   Log message in <ref>sprintf</ref> style format.
///   The resulting string after format should be no longer than <ref>LOG_LINE_LENGTH</ref> chars long.
/// </param>
/// <param name="...">Arguments to pass to <ref>sprintf</ref></param>
void Log(LogLevel logLevel, const char* messageFormat, ...)
{
	if (logLevel > Get_Current_Log_Level())
	{
		return;
	}

	auto messageBuffer = new char[LOG_LINE_LENGTH];
	auto messageWithLevelBuffer = new char[LOG_LINE_LENGTH + LOG_LEVEL_LENGTH];

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

	// output to log file and cleanup
	Append_To_File(LOG_FILE_PATH, messageWithLevelBuffer);

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
