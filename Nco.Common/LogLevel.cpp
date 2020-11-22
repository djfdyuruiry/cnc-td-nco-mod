#include "LogLevel.h"
#include "strings.h"
#include "utils.h"

const char* LogLevelToString(LogLevel level)
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

LogLevel ParseLogLevel(const char* levelString)
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
