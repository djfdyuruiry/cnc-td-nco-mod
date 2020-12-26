#include <string>
#include <windows.h>

#include "FileUtils.h"
#include "Logger.h"
#include "strings.h"
#include "utils.h"

Logger* Logger::INSTANCE = NULL;
const char* Logger::LOG_FORMAT = NULL;

void Logger::OpenLogFile()
{
	if (Win32HandleIsValid(logFileHandle) || failedToOpenLogFile)
	{
		return;
	}

	if (StringIsEmpty(logFilePath))
	{
		logFilePath = strdup("log\\nco.log");

		if (failedToOpenLogFile || StringIsEmpty(logFilePath))
		{
			return;
		}
	}

	bool errorOccurred = false;
	logFileHandle = FileUtils::OpenFileForAppending(logFilePath, &errorOccurred);

	if (errorOccurred || !Win32HandleIsValid(logFileHandle))
	{
		failedToOpenLogFile = true;

		WithWin32ErrorMessage([&](auto e) {
			ShowError("Failed to close handle for log file '%s': %s", logFilePath, e);
		});
	}
}

void Logger::CloseLogFileIfOpen()
{
	if (Win32HandleIsValid(logFileHandle))
	{
		LogDebug("Closing handle for log file: %s", logFilePath);
	}

	if (!CloseWin32HandleIfValid(logFileHandle))
	{
		WithWin32ErrorMessage([&](auto e) {
			LogError("Failed to close handle for log file '%s': %s", logFilePath, e);
		});
	}

	logFileHandle = NULL;
}

Logger::~Logger()
{
	CloseLogFileIfOpen();

	if (logFilePath != NULL)
	{
		delete logFilePath;
	}
}

void Logger::Log(LogLevel logLevel, const char* messageFormat, ...)
{
	if (logLevel > this->logLevel)
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
		LogLevelToString(logLevel),
		formattedMessage
	);

	delete formattedMessage;

	if (!failedToOpenLogFile && !Win32HandleIsValid(logFileHandle))
	{
		OpenLogFile();
	}

	// output to log file and console
	if (!failedToOpenLogFile)
	{
		FileUtils::AppendTextToFile(logFileHandle, logMessage);
	}

	if (!onlyLogErrorToStdOut || logLevel == ERR)
	{
		printf(logMessage);
	}

	delete logMessage;
}
