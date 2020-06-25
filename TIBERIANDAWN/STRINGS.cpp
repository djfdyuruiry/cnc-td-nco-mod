#include "function.h"

char* Allocate_String(int length)
{
	auto stringSizeInBytes = length * sizeof(char);
	auto string = new char[length];

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

bool Strings_Is_Empty(const char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, "");
}

bool Strings_Is_Empty(char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, "");
}

bool String_Starts_With(char* subject, char* expected)
{
	if (Strings_Is_Empty(subject) || Strings_Is_Empty(expected))
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
	if (csvString == NULL || Strings_Are_Equal(csvString, ""))
	{
		return NULL;
	}

	auto csvLength = strlen(csvString);

	if (csvLength < 1)
	{
		return NULL;
	}

	auto entryCount = 1;

	for (unsigned int i = 0; i < csvLength; i++)
	{
		if (csvString[i] == ',')
		{
			entryCount++;
		}
	}

	auto csvEntries = new char*[entryCount];

	auto currentEntryIdx = 0;
	auto currentEntryStrIdx = 0;

	csvEntries[currentEntryIdx] = Allocate_String(entrySize);

	for (unsigned int i = 0; i < csvLength + 1; i++)
	{
		// if not at a comma and have not reached the end of the houses string
		if (i != csvLength && csvString[i] != ',')
		{
			csvEntries[currentEntryIdx][currentEntryStrIdx] = csvString[i];
			currentEntryStrIdx++;

			continue;
		}

		currentEntryIdx++;
		currentEntryStrIdx = 0;

		csvEntries[currentEntryIdx] = Allocate_String(entrySize);
	}

	*csvEntryCount = entryCount;

	return (char**)csvEntries;
}
