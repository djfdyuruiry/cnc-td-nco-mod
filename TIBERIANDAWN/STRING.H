#ifndef STRING_H
#define STRING_H

char* Convert_String_To_Upper_Case(const char* subject, unsigned int subjectLength);

bool Strings_Are_Equal(const char* subject, const char* expected);

bool Strings_Are_Equal(char* subject, const char* expected);

char* Convert_String_To_Upper_Case(const char* subject);

void Convert_String_To_Upper_Case(char* subject);

char* Allocate_String(int length);

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount);

#endif
