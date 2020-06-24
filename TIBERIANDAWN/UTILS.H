#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

void Show_Error(const char* messageFormat, ...);

const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue);

char* Get_Win32_Error_Message(DWORD error);

char* Get_Win32_Error_Message();

HANDLE Open_File_For_Appending(const char* filename, bool* errorOccured);

void Append_To_File(HANDLE file, char* data);

int Parse_Number(char character);

bool Parse_Boolean(char* booleanValue);

bool Parse_Boolean(const char* booleanValue);

char * Convert_Boolean_To_String(bool booleanValue);

#endif
