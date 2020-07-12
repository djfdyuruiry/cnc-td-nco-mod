#include "function.h"

static const auto COMMA = ",";
static const auto EMPTY_STRING = "";

char* Allocate_String(unsigned int length)
{
	if (length == 0)
	{
		Show_Error("Length passed to Allocate_String was less than 1");

		return strdup(EMPTY_STRING);
	}

	auto stringSizeInBytes = (length + 1) * sizeof(char);
	auto string = (char*)malloc(stringSizeInBytes);

	memset(string, 0, stringSizeInBytes);

	return string;
}

bool Strings_Are_Equal(char* subject, char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	return strcmp(subject, expected) == 0;
}

bool Strings_Are_Equal(const char* subject, char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto subjectStr = strdup(subject);

	auto result = Strings_Are_Equal(subjectStr, expected);

	delete subjectStr;

	return result;
}

bool Strings_Are_Equal(char* subject, const char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto expectedStr = strdup(expected);

	auto result = Strings_Are_Equal(subject, expectedStr);
	
	delete expectedStr;

	return result;
}

bool Strings_Are_Equal(const char* subject, const char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = Strings_Are_Equal(subjectStr, expectedStr);
	
	delete subjectStr;
	delete expectedStr;

	return result;
}

bool String_Is_Empty(const char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, EMPTY_STRING);
}

bool String_Is_Empty(char* subject)
{
	return subject == NULL || Strings_Are_Equal(subject, EMPTY_STRING);
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
	if (String_Is_Empty(subject) || String_Is_Empty(expected))
	{
		return false;
	}

	auto subjectStr = strdup(subject);

	auto result = String_Starts_With(subjectStr, expected);

	delete subjectStr;

	return result;
}

bool String_Starts_With(char* subject, const char* expected)
{
	if (String_Is_Empty(subject) || String_Is_Empty(expected))
	{
		return false;
	}

	auto expectedStr = strdup(expected);

	auto result = String_Starts_With(subject, expectedStr);

	delete expectedStr;

	return result;
}

bool String_Starts_With(const char* subject, const char* expected)
{
	if (String_Is_Empty(subject) || String_Is_Empty(expected))
	{
		return false;
	}

	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = String_Starts_With(subjectStr, expectedStr);

	delete subjectStr;
	delete expectedStr;

	return result;
}

void Convert_String_To_Upper_Case(char* subject)
{
	if (String_Is_Empty(subject))
	{
		return;
	}

	_strupr(subject);
}

char* Convert_String_To_Upper_Case(const char* subject)
{
	if (String_Is_Empty(subject))
	{
		return strdup(EMPTY_STRING);
	}

	auto uppercaseSubject = strdup(subject);

	Convert_String_To_Upper_Case(uppercaseSubject);

	return uppercaseSubject;
}

char** Parse_Csv_String(char* csvString, unsigned int entrySize, int* csvEntryCount)
{
	if (String_Is_Empty(csvString))
	{
		Show_Error("CSV string passed to Parse_Csv_String was null or empty");

		*csvEntryCount = 0;

		return NULL;
	}

	// get a count for the csv list size
	auto countCsvStringBuffer = strdup(csvString);
	auto entryCount = 0;
	
	auto token = strtok(countCsvStringBuffer, COMMA);

	while (token != NULL) {
		token = strtok(NULL, COMMA);
		entryCount++;
	}

	delete countCsvStringBuffer;

	// read the csv entries into the new list of correct size
	auto csvEntries = new char* [entryCount];

	auto readStringBuffer = strdup(csvString);
	auto entryIdx = 0;

	token = strtok(readStringBuffer, COMMA);

	while(token != NULL && entryIdx < entryCount)
	{
		csvEntries[entryIdx] = token;
		entryIdx++;

		token = strtok(NULL, COMMA);
	}

	if (csvEntryCount != NULL)
	{
		*csvEntryCount = entryCount;
	}

	return csvEntries;
}
