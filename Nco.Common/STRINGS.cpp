#include <string>

#include "utils.h"

static auto COMMA = ",";
static auto EMPTY_STRING = "";

char* AllocateString(unsigned int length)
{
	if (length == 0)
	{
		ShowError("Length passed to AllocateString was less than 1");

		return strdup(EMPTY_STRING);
	}

	auto string = new char[length];

	string[0] = '\0';

	return string;
}

bool StringsAreEqual(char* subject, char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	return strcmp(subject, expected) == 0;
}

bool StringsAreEqual(const char* subject, char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto subjectStr = strdup(subject);

	auto result = StringsAreEqual(subjectStr, expected);

	delete subjectStr;

	return result;
}

bool StringsAreEqual(char* subject, const char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto expectedStr = strdup(expected);

	auto result = StringsAreEqual(subject, expectedStr);
	
	delete expectedStr;

	return result;
}

bool StringsAreEqual(const char* subject, const char* expected)
{
	if (subject == NULL || expected == NULL)
	{
		return false;
	}

	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = StringsAreEqual(subjectStr, expectedStr);
	
	delete subjectStr;
	delete expectedStr;

	return result;
}

bool StringIsEmpty(const char* subject)
{
	return subject == NULL || StringsAreEqual(subject, EMPTY_STRING);
}

bool StringIsEmpty(char* subject)
{
	return subject == NULL || StringsAreEqual(subject, EMPTY_STRING);
}

bool StringStartsWith(char* subject, char* expected)
{
	if (StringIsEmpty(subject) || StringIsEmpty(expected))
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

bool StringStartsWith(const char* subject, char* expected)
{
	if (StringIsEmpty(subject) || StringIsEmpty(expected))
	{
		return false;
	}

	auto subjectStr = strdup(subject);

	auto result = StringStartsWith(subjectStr, expected);

	delete subjectStr;

	return result;
}

bool StringStartsWith(char* subject, const char* expected)
{
	if (StringIsEmpty(subject) || StringIsEmpty(expected))
	{
		return false;
	}

	auto expectedStr = strdup(expected);

	auto result = StringStartsWith(subject, expectedStr);

	delete expectedStr;

	return result;
}

bool StringStartsWith(const char* subject, const char* expected)
{
	if (StringIsEmpty(subject) || StringIsEmpty(expected))
	{
		return false;
	}

	auto subjectStr = strdup(subject);
	auto expectedStr = strdup(expected);

	auto result = StringStartsWith(subjectStr, expectedStr);

	delete subjectStr;
	delete expectedStr;

	return result;
}

void ConvertStringToUpperCase(char* subject)
{
	if (StringIsEmpty(subject))
	{
		return;
	}

	_strupr(subject);
}

char* ConvertStringToUpperCase(const char* subject)
{
	if (StringIsEmpty(subject))
	{
		return strdup(subject);
	}

	auto uppercaseSubject = strdup(subject);

	ConvertStringToUpperCase(uppercaseSubject);

	return uppercaseSubject;
}

char** ParseCsvString(const char* csvString, unsigned int entrySize, unsigned int* csvEntryCount, const char* seperator = COMMA)
{
	if (StringIsEmpty(csvString))
	{
		ShowError("CSV string passed to ParseCsvString was null or empty");

		*csvEntryCount = 0u;

		return NULL;
	}

	// get a count for the csv list size
	auto countCsvStringBuffer = strdup(csvString);
	auto entryCount = 0u;
	
	auto token = strtok(countCsvStringBuffer, seperator);

	while (token != NULL) {
		token = strtok(NULL, seperator);
		entryCount++;
	}

	delete countCsvStringBuffer;

	// read the csv entries into the new list of correct size
	auto csvEntries = new char* [entryCount];

	auto readStringBuffer = strdup(csvString);
	auto entryIdx = 0u;

	token = strtok(readStringBuffer, seperator);

	while(token != NULL && entryIdx < entryCount)
	{
		csvEntries[entryIdx] = token;
		entryIdx++;

		token = strtok(NULL, seperator);
	}

	if (csvEntryCount != NULL)
	{
		*csvEntryCount = entryCount;
	}

	return csvEntries;
}

char* FormatString(const char* format, unsigned int maxLength, va_list formatArgs)
{
	auto stringBuffer = AllocateString(maxLength);

	vsnprintf(stringBuffer, maxLength, format, formatArgs);

	return stringBuffer;
}

char* FormatString(const char* format, va_list formatArgs)
{
	return FormatString(format, 1024, formatArgs);
}

char* FormatString(const char* format, unsigned  int maxLength, ...)
{
	va_list formatArgs;
	va_start(formatArgs, maxLength);

	auto stringBuffer = FormatString(format, maxLength, formatArgs);

	va_end(formatArgs);

	return stringBuffer;
}

char* FormatString(const char* format, ...)
{
	va_list formatArgs;
	va_start(formatArgs, format);

	auto stringBuffer = FormatString(format, formatArgs);

	va_end(formatArgs);

	return stringBuffer;
}

char* ToTitleCase(const char* subject)
{
	if (subject == NULL)
	{
		return NULL;
	}
	else if (StringIsEmpty(subject))
	{
		return strdup(subject);
	}

	auto titleCaseSubject = strdup(subject);

	titleCaseSubject[0] = toupper(subject[0]);

	return titleCaseSubject;
}

char* ExtractSubstring(const char* subject, int substringLength, int startIndex = 0)
{
	auto inputLength = (int)strlen(subject);

	if (startIndex < 0)
	{
		startIndex = inputLength + startIndex;
	}

	if (substringLength < 0)
	{
		substringLength = inputLength + substringLength;
	}

	if (startIndex >= inputLength || substringLength >= inputLength)
	{
		return strdup(subject);
	}

	if (startIndex == 0 && substringLength == 0)
	{
		if (substringLength < 1)
		{
			return strdup(EMPTY_STRING);
		}

		return AllocateString(substringLength);
	}

	auto substring = AllocateString(substringLength);

	strncpy(substring, subject, substringLength);

	substring[substringLength] = '\0';

	return substring;
}

char* RepeatString(const char* subject, unsigned int times)
{
	if (times == 0u)
	{
		if (subject == NULL)
		{
			return strdup(EMPTY_STRING);
		}

		return AllocateString(strlen(subject));
	}

	if (times == 1u)
	{
		return strdup(subject);
	}

	auto interval = strlen(subject);
	auto totalLength = interval * times;
	auto repeatedString = AllocateString(totalLength);

	for (auto offset = 0u; offset < totalLength - 1; offset += interval)
	{
		strncpy(
			repeatedString + (offset * sizeof(char)),
			subject,
			interval
		);
	}

	repeatedString[totalLength + 1] = '\0';

	return repeatedString;
}
