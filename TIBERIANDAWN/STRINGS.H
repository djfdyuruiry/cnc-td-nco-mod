#ifndef STRING_H
#define STRING_H

bool Strings_Are_Equal(char* subject, char* expected);

bool Strings_Are_Equal(const char* subject, char* expected);

bool Strings_Are_Equal(char* subject, const char* expected);

#define Convert_String_To_Upper_Case(subject) _strupr(subject)

char* Convert_String_To_Upper_Case(const char* subject);

char* Allocate_String(int length);

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount);

#endif
