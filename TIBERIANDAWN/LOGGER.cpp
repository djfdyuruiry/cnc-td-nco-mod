#include "function.h"

static auto LOG_FILE_PATH = "log\\nco.log";
static auto LOG_LINE_LENGTH = 1024;
static auto LOG_LEVEL_LENGTH = 8;

const char* LogLevel_To_String(LogLevel level)
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
		LogLevel_To_String(logLevel),
		messageBuffer
	);

	// output to log file and cleanup
	Append_To_File(LOG_FILE_PATH, messageWithLevelBuffer);

	delete messageBuffer;
	delete messageWithLevelBuffer;
}
