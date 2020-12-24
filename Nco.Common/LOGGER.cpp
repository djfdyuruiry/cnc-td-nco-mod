#include <shlobj.h>
#include <string>
#include <windows.h>

#include "FileUtils.h"
#include "Logger.h"
#include "strings.h"
#include "utils.h"

Logger* Logger::INSTANCE = NULL;
const char* Logger::LOG_FORMAT = NULL;

void Logger::LoadDefaultLogFilePath()
{
	auto documentsPath = AllocateString(MAX_PATH);
	auto result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentsPath);

	if (FAILED(result))
	{
		ShowError("Failed to read user documents path, logging to file will be disabled");
		failedToOpenLogFile = true;

		return;
	}

	auto cncPath = FormatString("%s\\CnCRemastered", MAX_PATH, documentsPath);

	delete documentsPath;

	if (!FileUtils::IsDirectory(cncPath))
	{
		failedToOpenLogFile = true;

		LogError("CNC Remastered document path has not been created yet, logging to file will be disabled. Path: %s", cncPath);

		delete cncPath;

		return;
	}

	logFilePath = FormatString("%s\\nco.log", MAX_PATH, cncPath);

	delete cncPath;
}

void Logger::OpenLogFile()
{
	if (Win32HandleIsValid(logFileHandle) || failedToOpenLogFile) {
		return;
	}

	if (StringIsEmpty(logFilePath))
	{
		LoadDefaultLogFilePath();
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
			ShowError("Failed to close handle for log file '%s': %s", logFilePath, e);
			});
	}

	logFileHandle = NULL;
}

Logger::~Logger()
{
	CloseLogFileIfOpen();

	delete logFilePath;
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
