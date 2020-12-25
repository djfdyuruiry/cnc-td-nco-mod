#pragma once

#include <functional>
#include <windows.h>

void ShowError(const char* messageFormat, va_list formatArgs);

void ShowError(const char* messageFormat, ...);

void ShowErrorAndExit(const char* messageFormat, va_list formatArgs);

void ShowErrorAndExit(const char* messageFormat, ...);

const char* GetEnvVar(const char* envVarName, bool* valueFound);

char* GetWin32ErrorMessage(DWORD error);

char* GetWin32ErrorMessage();

void WithWin32ErrorMessage(std::function<void(const char*)> handler);

bool IsIntString(char* numberValue);

bool IsIntString(const char* numberValue);

bool IsUnsignedIntString(char* numberValue);

bool IsUnsignedIntString(const char* numberValue);

bool IsDoubleString(char* numberValue);

int ParseNumber(char character);

char* ConvertNumberToString(int number);

bool IsBooleanString(char* booleanValue);

bool ParseBoolean(char* booleanValue, bool* parseError);

bool ParseBoolean(const char* booleanValue, bool* parseError);

bool ParseBooleanOrDefault(char* booleanValue, bool default);

bool ParseBooleanOrDefault(const char* booleanValue, bool default);

const char* ConvertBooleanToString(bool booleanValue);

long long GetNowInEpochNanos();

long long GetNowInEpochMillis();

long long GetNowInEpochSecs();

const char* GetModDataPath();

char* BuildModDataFilePath(const char* filePath);

void StartConsoleOutput();

void StopConsoleOutput();

bool Win32HandleIsValid(HANDLE handle);

bool CloseWin32HandleIfValid(HANDLE handle);
