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

	if (StringsAreEqual(levelString, "TRACE"))
	{
		logLevel = TRACE;
	}
	else if (StringsAreEqual(levelString, "DEBUG"))
	{
		logLevel = DEBUG;
	}
	else if (StringsAreEqual(levelString, "WARN"))
	{
		logLevel = WARN;
	}
	else if (StringsAreEqual(levelString, "ERROR"))
	{
		logLevel = ERR;
	}
	else if (StringsAreEqual(levelString, "OFF"))
	{
		logLevel = OFF;
	}
	else if (!StringsAreEqual(levelString, "INFO"))
	{
		ShowError("Setting log level to INFO as an unrecognised log level was provided: %s", levelString);
	}

	return logLevel;
}
