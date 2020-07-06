#ifndef STRING_H
#define STRING_H

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

char* Convert_Number_To_String(int number);

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount);

#endif
