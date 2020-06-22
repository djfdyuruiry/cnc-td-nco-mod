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

char* Allocate_String(int length)
{
	auto stringSizeInBytes = length * sizeof(char);
	auto string = (char*)malloc(stringSizeInBytes);

	memset(string, 0, stringSizeInBytes);

	return string;
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
