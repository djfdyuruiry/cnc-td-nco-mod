#include "CppUnitTest.h"

#include "strings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	TEST_CLASS(StringUtilsTests)
	{
		public:
			TEST_METHOD(When_Allocate_String_IsCalled_WithNonZeroLength_ThenEmptyStringIsReturned)
			{
				auto testStr = Allocate_String(10u);

				Assert::AreEqual("", testStr);
			}

			TEST_METHOD(When_Allocate_String_IsCalled_WithNonZeroLength_ThenStringCanBeWrittenTo)
			{
				auto testStr = Allocate_String(10u);

				sprintf(testStr, "1234567890");

				Assert::AreEqual("1234567890", testStr);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithMatchingNonConstStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					Strings_Are_Equal(strdup("hello"), strdup("hello"))
				);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithNonMatchingNonConstStrings_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal(strdup("fetch"), strdup("stay"))
				);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithMatchingConstStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					Strings_Are_Equal("hello", "hello")
				);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithNonMatchingConstStrings_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal("hallo", "hej")
				);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithNullSubject_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal((char*)NULL, "nope")
				);
			}

			TEST_METHOD(When_Strings_Are_Equal_IsCalled_WithNullExpected_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal("negative", (char*)NULL)
				);
			}

			TEST_METHOD(When_String_Is_Empty_IsCalled_WithBlankConstSubject_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					String_Is_Empty("")
				);
			}

			TEST_METHOD(When_String_Is_Empty_IsCalled_WithBlankNonConstSubject_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					String_Is_Empty(strdup(""))
				);
			}

			TEST_METHOD(When_String_Is_Empty_IsCalled_WithNonBlankConstSubject_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Is_Empty("i'm not dead")
				);
			}

			TEST_METHOD(When_String_Is_Empty_IsCalled_WithNonBlankNonConstSubject_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Is_Empty(strdup("i'm not dead"))
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithNonConstMatchingStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					String_Starts_With(strdup("what the"), strdup("wha"))
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithNonConstNonMatchingStrings_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Starts_With(strdup("where"), strdup("wha"))
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithConstMatchingStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					String_Starts_With("juice", "jui")
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithConstNonMatchingStrings_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Starts_With("plane", "car")
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithNullSubject_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Starts_With((char*)NULL, "abs")
				);
			}

			TEST_METHOD(When_String_Starts_With_IsCalled_WithNullExpected_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					String_Starts_With("lake", (char*)NULL)
				);
			}

			TEST_METHOD(When_Convert_String_To_Upper_Case_IsCalled_WithNonBlankString_ThenUppercaseStringIsReturned)
			{
				auto testStr = strdup("Add Read");

				Convert_String_To_Upper_Case(testStr);

				Assert::AreEqual("ADD READ", testStr);
			}

			TEST_METHOD(When_Convert_String_To_Upper_Case_IsCalled_WithNonBlankConstString_ThenUppercaseStringIsReturned)
			{
				auto result = Convert_String_To_Upper_Case("lo,WerDea,sisH");

				Assert::AreEqual("LO,WERDEA,SISH", result);
			}

			TEST_METHOD(When_Parse_Csv_String_IsCalled_WithStringWithOneEntry_ThenCountOfOneIsReturned)
			{
				auto entryCount = 0;
				
				Parse_Csv_String(strdup("entry_0"), 7, &entryCount);

				Assert::AreEqual(1, entryCount);
			}

			TEST_METHOD(When_Parse_Csv_String_IsCalled_WithStringWithOneEntry_ThenEntryIsReturned)
			{
				auto entryCount = 0;
				auto result = Parse_Csv_String(strdup("entry_0"), 7, &entryCount);

				auto ewq = strcmp(strdup("entry_0"), result[0]);
				printf("Help me god %d", ewq);

				Assert::AreEqual("entry_0", result[0]);
			}

			TEST_METHOD(When_Parse_Csv_String_IsCalled_WithStringWithMultipleEntries_ThenEntryCountIsCorrect)
			{
				auto entryCount = 0;
				
				Parse_Csv_String(strdup("entry_0,entry_1,entry_2,entry_3"), 7, &entryCount);

				Assert::AreEqual(4, entryCount);
			}

			TEST_METHOD(When_Parse_Csv_String_IsCalled_WithStringWithMultipleEntries_ThenEntriesAreReturned)
			{
				auto entryCount = 0;
				auto result = Parse_Csv_String(strdup("entry_0,entry_1,entry_2,entry_3"), 7, &entryCount);

				Assert::AreEqual("entry_0", result[0]);
				Assert::AreEqual("entry_1", result[1]);
				Assert::AreEqual("entry_2", result[2]);
				Assert::AreEqual("entry_3", result[3]);
			}
	};
}
