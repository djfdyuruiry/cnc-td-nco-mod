#pragma once

#include <windows.h>

#include "LogLevel.h"

class Logger final
{
private:
	static const char* LOG_FORMAT;
	static const auto LOG_LINE_LENGTH = 25600u;
	static const auto LOG_LEVEL_LENGTH = 5u;
	static const auto LOG_TIMESTAMP_LENGTH = 20u;
	static const auto LOG_SPACING_LENGTH = 4u;
	static const auto LOG_FORMAT_LENGTH = LOG_TIMESTAMP_LENGTH + LOG_SPACING_LENGTH + LOG_LEVEL_LENGTH + LOG_LINE_LENGTH;

	static Logger* INSTANCE;

	LogLevel logLevel;
	const char* logFilePath;
	HANDLE logFileHandle;

	bool onlyLogErrorToStdOut;
	bool failedToOpenLogFile;

	Logger() : logLevel(OFF), logFilePath(NULL), logFileHandle(NULL), onlyLogErrorToStdOut(false), failedToOpenLogFile(false)
	{
	}

	void LoadDefaultLogFilePath();

	void OpenLogFile();

	void CloseLogFileIfOpen();

public:
	static Logger& GetInstance()
	{
		if (INSTANCE == NULL)
		{
			LOG_FORMAT = "%02d-%02d-%04d %02d:%02d:%02d.%03d - %s %s\n";
			INSTANCE = new Logger();
		}

		return *INSTANCE;
	}

	static void Shutdown()
	{
		if (INSTANCE == NULL)
		{
			return;
		}

		delete INSTANCE;

		INSTANCE = NULL;
	}

	static unsigned int GetLogLineLength()
	{
		return LOG_LINE_LENGTH;
	}

	~Logger();

	const char* GetLogFilePath()
	{
		return logFilePath;
	}

	void SetLogFilePath(const char* logFilePath)
	{
		this->logFilePath = logFilePath;
	}

	LogLevel GetLogLevel()
	{
		return logLevel;
	}

	void SetLogLevel(LogLevel level)
	{
		logLevel = level;
	}

	bool ConsoleLoggingIsEnabled()
	{
		return !onlyLogErrorToStdOut;
	}

	void ToggleConsoleLogging()
	{
		onlyLogErrorToStdOut = !onlyLogErrorToStdOut;
	}

	void Log(LogLevel level, const char* messageFormat, ...);

};

#define GetLogger() Logger::GetInstance()

#define LogError(messageFormat, ...) GetLogger().Log(ERR, messageFormat, __VA_ARGS__)

#define LogWarn(messageFormat, ...) GetLogger().Log(WARN, messageFormat, __VA_ARGS__)

#define LogInfo(messageFormat, ...) GetLogger().Log(INFO, messageFormat, __VA_ARGS__)

#define LogDebug(messageFormat, ...) GetLogger().Log(DEBUG, messageFormat, __VA_ARGS__)

#define LogTrace(messageFormat, ...) GetLogger().Log(TRACE, messageFormat, __VA_ARGS__)
