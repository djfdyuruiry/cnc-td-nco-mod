#include "function.h"

char* Allocate_String(int length)
{
	auto stringSizeInBytes = (length  + 1) * sizeof(char);
	auto string = (char*)malloc(stringSizeInBytes);

	memset(string, 0, stringSizeInBytes);

	return string;
}

bool Strings_Are_Equal(char* subject, char* expected)
{
	return strcmp(subject, expected) == 0;
}

bool Strings_Are_Equal(const char* subject, char* expected)
{
	auto subjectStr = strdup(subject);

	auto result = Strings_Are_Equal(subjectStr, expected);

	delete subjectStr;

	return result;
}

bool Strings_Are_Equal(char* subject, const char* expected)
{
	auto expectedStr = strdup(expected);

	auto result = Strings_Are_Equal(subject, expectedStr);
	
	delete expectedStr;

	return result;
}

bool Strings_Are_Equal(const char* subject, const char* expected)
{
	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = Strings_Are_Equal(subjectStr, expectedStr);
	
	delete subjectStr;
	delete expectedStr;

	return result;
}

bool String_Is_Empty(const char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, "");
}

bool String_Is_Empty(char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, "");
}

bool String_Starts_With(char* subject, char* expected)
{
	if (String_Is_Empty(subject) || String_Is_Empty(expected))
	{
		return false;
	}

	auto subjectLength = strlen(subject);
	auto expectedLength = strlen(expected);

	if (expectedLength > subjectLength)
	{
		return false;
	}

	for (unsigned int i = 0; i < expectedLength; i++)
	{
		if (subject[i] != expected[i])
		{
			return false;
		}
	}

	return true;
}

bool String_Starts_With(const char* subject, char* expected)
{
	auto subjectStr = strdup(subject);

	auto result = String_Starts_With(subjectStr, expected);

	delete subjectStr;

	return result;
}
bool String_Starts_With(char* subject, const char* expected)
{
	auto expectedStr = strdup(expected);

	auto result = String_Starts_With(subject, expectedStr);

	delete expectedStr;

	return result;
}

bool String_Starts_With(const char* subject, const char* expected)
{
	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = String_Starts_With(subjectStr, expectedStr);

	delete subjectStr;
	delete expectedStr;

	return result;
}

char* Convert_String_To_Upper_Case(const char* subject)
{
	auto uppercaseSubject = strdup(subject);

	Convert_String_To_Upper_Case(uppercaseSubject);

	return uppercaseSubject;
}

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount)
{
	if (String_Is_Empty(csvString))
	{
		return NULL;
	}

	auto entryCount = 0;
	
	auto token = strtok(csvString, ",");

	while (token != NULL) {
		token = strtok(NULL, ",");
		entryCount++;
	}

	auto csvEntries = new char*[entryCount];

	token = strtok(csvString, ",");

	for (auto i = 0; i < entryCount; i++)
	{
		csvEntries[i] = strtok(NULL, ",");
	}

	return csvEntries;
}
