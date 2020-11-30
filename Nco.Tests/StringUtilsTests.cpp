#include "CppUnitTest.h"

#include <strings.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	namespace Nco
	{
		namespace Common
		{
			TEST_CLASS(StringUtilsTests)
			{
			public:
				TEST_METHOD(When_AllocateString_IsCalled_WithNonZeroLength_ThenEmptyStringIsReturned)
				{
					auto testStr = AllocateString(10u);

					Assert::AreEqual("", testStr);
				}

				TEST_METHOD(When_AllocateString_IsCalled_WithNonZeroLength_ThenStringCanBeWrittenTo)
				{
					auto testStr = AllocateString(10u);

					sprintf(testStr, "1234567890");

					Assert::AreEqual("1234567890", testStr);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithMatchingNonConstStrings_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringsAreEqual(strdup("hello"), strdup("hello"))
					);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithNonMatchingNonConstStrings_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringsAreEqual(strdup("fetch"), strdup("stay"))
					);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithMatchingConstStrings_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringsAreEqual("hello", "hello")
					);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithNonMatchingConstStrings_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringsAreEqual("hallo", "hej")
					);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithNullSubject_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringsAreEqual((char*)NULL, "nope")
					);
				}

				TEST_METHOD(When_StringsAreEqual_IsCalled_WithNullExpected_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringsAreEqual("negative", (char*)NULL)
					);
				}

				TEST_METHOD(When_StringIsEmpty_IsCalled_WithBlankConstSubject_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringIsEmpty("")
					);
				}

				TEST_METHOD(When_StringIsEmpty_IsCalled_WithBlankNonConstSubject_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringIsEmpty(strdup(""))
					);
				}

				TEST_METHOD(When_StringIsEmpty_IsCalled_WithNonBlankConstSubject_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringIsEmpty("i'm not dead")
					);
				}

				TEST_METHOD(When_StringIsEmpty_IsCalled_WithNonBlankNonConstSubject_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringIsEmpty(strdup("i'm not dead"))
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithNonConstMatchingStrings_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringStartsWith(strdup("what the"), strdup("wha"))
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithNonConstNonMatchingStrings_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringStartsWith(strdup("where"), strdup("wha"))
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithConstMatchingStrings_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						StringStartsWith("juice", "jui")
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithConstNonMatchingStrings_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringStartsWith("plane", "car")
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithNullSubject_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringStartsWith((char*)NULL, "abs")
					);
				}

				TEST_METHOD(When_StringStartsWith_IsCalled_WithNullExpected_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						StringStartsWith("lake", (char*)NULL)
					);
				}

				TEST_METHOD(When_ConvertStringToUpperCase_IsCalled_WithNonBlankString_ThenUppercaseStringIsReturned)
				{
					auto testStr = strdup("Add Read");

					ConvertStringToUpperCase(testStr);

					Assert::AreEqual("ADD READ", testStr);
				}

				TEST_METHOD(When_ConvertStringToUpperCase_IsCalled_WithNonBlankConstString_ThenUppercaseStringIsReturned)
				{
					auto result = ConvertStringToUpperCase("lo,WerDea,sisH");

					Assert::AreEqual("LO,WERDEA,SISH", result);
				}

				TEST_METHOD(When_ParseCsvString_IsCalled_WithStringWithOneEntry_ThenCountOfOneIsReturned)
				{
					auto entryCount = 0u;

					ParseCsvString(strdup("entry_0"), 7, &entryCount);

					Assert::AreEqual(1u, entryCount);
				}

				TEST_METHOD(When_ParseCsvString_IsCalled_WithStringWithOneEntry_ThenEntryIsReturned)
				{
					auto entryCount = 0u;
					auto result = ParseCsvString(strdup("entry_0"), 7, &entryCount);

					Assert::AreEqual("entry_0", result[0]);
				}

				TEST_METHOD(When_ParseCsvString_IsCalled_WithStringWithMultipleEntries_ThenEntryCountIsCorrect)
				{
					auto entryCount = 0u;

					ParseCsvString(strdup("entry_0,entry_1,entry_2,entry_3"), 7, &entryCount);

					Assert::AreEqual(4u, entryCount);
				}

				TEST_METHOD(When_ParseCsvString_IsCalled_WithStringWithMultipleEntries_ThenEntriesAreReturned)
				{
					auto entryCount = 0u;
					auto result = ParseCsvString(strdup("entry_0,entry_1,entry_2,entry_3"), 7, &entryCount);

					Assert::AreEqual("entry_0", result[0]);
					Assert::AreEqual("entry_1", result[1]);
					Assert::AreEqual("entry_2", result[2]);
					Assert::AreEqual("entry_3", result[3]);
				}

				TEST_METHOD(When_FormatString_IsCalled_WithNonEmptyString_AndFormatArgs_ThenFormattedStringIsReturned)
				{
					auto result = FormatString("This should have %s and %d in it", "this", 294);

					Assert::AreEqual("This should have this and 294 in it", result);
				}

				TEST_METHOD(When_ToTitleCase_IsCalled_WithNonEmptyString_ThenTitleCaseStringIsReturned)
				{
					auto result = ToTitleCase("tardis");

					Assert::AreEqual("Tardis", result);
				}

			};
		}
	}
}
