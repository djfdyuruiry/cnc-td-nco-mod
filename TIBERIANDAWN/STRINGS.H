#ifndef STRING_H
#define STRING_H

char* Allocate_String(int length);

bool Strings_Are_Equal(char* subject, char* expected);

bool Strings_Are_Equal(const char* subject, char* expected);

bool Strings_Are_Equal(char* subject, const char* expected);

bool String_Is_Empty(char* subject);

bool String_Is_Empty(const char* subject);

bool String_Starts_With(char* subject, char* expected);

bool String_Starts_With(const char* subject, char* expected);

bool String_Starts_With(char* subject, const char* expected);

bool String_Starts_With(const char* subject, const char* expected);

#define Convert_String_To_Upper_Case(subject) _strupr(subject)

char* Convert_String_To_Upper_Case(const char* subject);

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount);

#endif
