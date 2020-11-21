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

const char* Log_Level_To_String(LogLevel level);

LogLevel Parse_Log_Level(const char* levelString);

int Get_Log_Line_Length();

int Get_Log_Level_Length();

void Set_Current_Log_Path(const char* path);

const char* Current_Log_Path();

void Set_Current_Log_Level(LogLevel level);

LogLevel Current_Log_Level();

void Log(LogLevel logLevel, const char* messageFormat, ...);

#define Log_Error(messageFormat, ...) Log(ERR, messageFormat, __VA_ARGS__)

#define Log_Warn(messageFormat, ...) Log(WARN, messageFormat, __VA_ARGS__)

#define Log_Info(messageFormat, ...) Log(INFO, messageFormat, __VA_ARGS__)

#define Log_Debug(messageFormat, ...) Log(DEBUG, messageFormat, __VA_ARGS__)

#define Log_Trace(messageFormat, ...) Log(TRACE, messageFormat, __VA_ARGS__)

void Toggle_Console_Logging();

bool Console_Logging_Enabled();

void Close_Log_File_If_Open();
