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
	auto levelStringUppercase = ConvertStringToUpperCase(levelString);
	auto logLevel = INFO;

	if (StringsAreEqual(levelStringUppercase, "TRACE"))
	{
		logLevel = TRACE;
	}
	else if (StringsAreEqual(levelStringUppercase, "DEBUG"))
	{
		logLevel = DEBUG;
	}
	else if (StringsAreEqual(levelStringUppercase, "WARN"))
	{
		logLevel = WARN;
	}
	else if (StringsAreEqual(levelStringUppercase, "ERROR"))
	{
		logLevel = ERR;
	}
	else if (StringsAreEqual(levelStringUppercase, "OFF"))
	{
		logLevel = OFF;
	}
	else if (!StringsAreEqual(levelStringUppercase, "INFO"))
	{
		ShowError("Setting log level to INFO as an unrecognised log level was provided: %s", levelString);
	}

	delete levelStringUppercase;

	return logLevel;
}
