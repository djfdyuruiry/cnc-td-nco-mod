#pragma once

char* Allocate_String(unsigned int length);

bool Strings_Are_Equal(char* subject, char* expected);

bool Strings_Are_Equal(const char* subject, char* expected);

bool Strings_Are_Equal(char* subject, const char* expected);

bool Strings_Are_Equal(const char* subject, const char* expected);

bool String_Is_Empty(char* subject);

bool String_Is_Empty(const char* subject);

bool String_Starts_With(char* subject, char* expected);

bool String_Starts_With(const char* subject, char* expected);

bool String_Starts_With(char* subject, const char* expected);

bool String_Starts_With(const char* subject, const char* expected);

void Convert_String_To_Upper_Case(char* subject);

char* Convert_String_To_Upper_Case(const char* subject);

char** Parse_Csv_String(char* csvString, unsigned int entrySize, unsigned int* csvEntryCount);

char* FormatString(const char* format, int maxLength, va_list formatArgs);

char* FormatString(const char* format, va_list formatArgs);

char* FormatString(const char* format, int maxLength, ...);

char* FormatString(const char* format, ...);

char* ToTitleCase(const char* subject);
