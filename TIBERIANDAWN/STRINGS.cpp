#include "function.h"

bool Strings_Are_Equal(char* subject, const char* expected)
{
	return strcmp(subject, expected) == 0;
}

void Convert_String_To_Upper_Case(char* subject)
{
	auto subjectLength = strlen(subject);

	for (unsigned int i = 0; i < subjectLength; i++)
	{
		subject[i] = toupper(subject[i]);
	}
}
