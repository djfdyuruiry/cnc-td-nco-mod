#ifndef LOGGER_H
#define LOGGER_H

typedef enum LogLevel : unsigned short {
	LOG_LEVEL_COUNT = 6,
	TRACE = 5,
	DEBUG = 4,
	INFO = 3,
	WARN = 2,
	ERR = 1,
	OFF = 0
} LogLevel;

const char* Log_Level_To_String(LogLevel level);

LogLevel Parse_Log_Level(char* levelString);

LogLevel Parse_Log_Level(const char* levelString);

int Get_Log_Line_Length();

int Get_Log_Level_Length();

void Log(LogLevel logLevel, const char* messageFormat, ...);

#define Log_Error(messageFormat, ...) Log(ERR, messageFormat, __VA_ARGS__)

#define Log_Warn(messageFormat, ...) Log(WARN, messageFormat, __VA_ARGS__)

#define Log_Info(messageFormat, ...) Log(INFO, messageFormat, __VA_ARGS__)

#define Log_Debug(messageFormat, ...) Log(DEBUG, messageFormat, __VA_ARGS__)

#define Log_Trace(messageFormat, ...) Log(TRACE, messageFormat, __VA_ARGS__)

void Toggle_Console_Logging();

void Close_Log_File_If_Open();

#endif
