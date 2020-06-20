#include "function.h"

auto LOG_FILE_PATH = "log\\nco.log";
auto LOG_LINE_LENGTH = 1024;
auto LOG_LEVEL_LENGTH = 8;

void Log(const char* level, const char* messageFormat, va_list formatArgs)
{
	auto messageBuffer = new char[LOG_LINE_LENGTH];
	auto messageWithLevelBuffer = new char[LOG_LINE_LENGTH + LOG_LEVEL_LENGTH];

	vsnprintf(messageBuffer, LOG_LINE_LENGTH, messageFormat, formatArgs);

	SYSTEMTIME now;
	GetSystemTime(&now);

	sprintf(
		messageWithLevelBuffer,
		"%02d-%02d-%04d %02d:%02d - %s %s\n",
		now.wDay,
		now.wMonth,
		now.wYear,
		now.wHour,
		now.wMinute,
		level,
		messageBuffer
	);

	Append_To_File(LOG_FILE_PATH, messageWithLevelBuffer);

	delete messageBuffer;
}

void Log_Error(const char* messageFormat, ...)
{
	va_list argptr;
	va_start(argptr, messageFormat);

	Log("ERROR", messageFormat, argptr);

	va_end(argptr);
}

void Log_Warn(const char* messageFormat, ...)
{
	va_list argptr;
	va_start(argptr, messageFormat);

	Log("WARN", messageFormat, argptr);

	va_end(argptr);
}

void Log_Info(const char* messageFormat, ...)
{
	va_list argptr;
	va_start(argptr, messageFormat);

	Log("INFO", messageFormat, argptr);

	va_end(argptr);
}

void Log_Debug(const char* messageFormat, ...)
{
	va_list argptr;
	va_start(argptr, messageFormat);

	Log("DEBUG", messageFormat, argptr);

	va_end(argptr);
}
