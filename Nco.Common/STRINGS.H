#pragma once

#include <stdlib.h>

char* AllocateString(unsigned int length);

bool StringsAreEqual(char* subject, char* expected);

bool StringsAreEqual(const char* subject, char* expected);

bool StringsAreEqual(char* subject, const char* expected);

bool StringsAreEqual(const char* subject, const char* expected);

bool StringIsEmpty(char* subject);

bool StringIsEmpty(const char* subject);

bool StringStartsWith(char* subject, char* expected);

bool StringStartsWith(const char* subject, char* expected);

bool StringStartsWith(char* subject, const char* expected);

bool StringStartsWith(const char* subject, const char* expected);

void ConvertStringToUpperCase(char* subject);

char* ConvertStringToUpperCase(const char* subject);

char** ParseCsvString(const char* csvString, unsigned int entrySize, unsigned int* csvEntryCount, const char* seperator = ",");

char* FormatString(const char* format, unsigned int maxLength, va_list formatArgs);

char* FormatString(const char* format, va_list formatArgs);

char* FormatString(const char* format, unsigned int maxLength, ...);

char* FormatString(const char* format, ...);

char* ToTitleCase(const char* subject);

char* ExtractSubstring(const char* subject, int substringLength, int startIndex = 0);

char* RepeatString(const char* subject, unsigned int times);
