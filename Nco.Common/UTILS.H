#pragma once

#include <windows.h>

void Show_Error(const char* messageFormat, ...);

const char* Get_Env_Var(const char* envVarName, bool* valueFound);

char* Get_Win32_Error_Message(DWORD error);

char* Get_Win32_Error_Message();

HANDLE Open_File_For_Appending(const char* filename, bool* errorOccured);

void Append_To_File(HANDLE file, char* data);

bool Is_Int_String(char* numberValue);

bool Is_Int_String(const char* numberValue);

bool Is_Unsigned_Int_String(char* numberValue);

bool Is_Unsigned_Int_String(const char* numberValue);

bool Is_Double_String(char* numberValue);

int Parse_Number(char character);

char* Convert_Number_To_String(int number);

bool Is_Boolean_String(char* booleanValue);

bool Parse_Boolean(char* booleanValue, bool* parseError);

bool Parse_Boolean(const char* booleanValue, bool* parseError);

bool Parse_Boolean_Or_Default(char* booleanValue, bool default);

bool Parse_Boolean_Or_Default(const char* booleanValue, bool default);

const char* Convert_Boolean_To_String(bool booleanValue);

long long Get_Now_In_Epoch_Nanos();

long long Get_Now_In_Epoch_Millis();

long long Get_Now_In_Epoch_Secs();

const char* Get_Mod_Data_Path();

char* Build_Mod_Data_File_Path(const char* filePath);

void Start_Console_Output();

void Stop_Console_Output();

bool Win32HandleIsValid(HANDLE handle);

void CloseWin32HandleIfValid(HANDLE handle);
