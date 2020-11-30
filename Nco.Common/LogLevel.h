#pragma once

enum LogLevel : unsigned short {
	OFF,
	ERR,
	WARN,
	INFO,
	DEBUG,
	TRACE,
	LOG_LEVEL_COUNT
};

const char* LogLevelToString(LogLevel level);

LogLevel ParseLogLevel(const char* levelString);
