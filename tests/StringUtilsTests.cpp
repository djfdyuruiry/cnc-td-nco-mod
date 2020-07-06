#include "pch.h"
#include "CppUnitTest.h"

#include "strings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	TEST_CLASS(StringUtilsTests)
	{
		public:
			TEST_METHOD(WhenAllocate_StringIsCalled_WithNonZeroLength_ThenEmptyStringIsReturned)
			{
				auto testStr = Allocate_String(10u);

				Assert::AreEqual("", testStr);
			}

			TEST_METHOD(WhenAllocate_StringIsCalled_WithNonZeroLength_ThenStringCanBeWrittenTo)
			{
				auto testStr = Allocate_String(10u);

				sprintf(testStr, "1234567890");

				Assert::AreEqual("1234567890", testStr);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithMatchingNonConstStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					Strings_Are_Equal(strdup("hello"), strdup("hello"))
				);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithNonMatchingNonConstStrings_ThenFalseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal(strdup("fetch"), strdup("stay"))
				);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithMatchingConstStrings_ThenTrueIsReturned)
			{
				Assert::AreEqual(
					true,
					Strings_Are_Equal("hello", "hello")
				);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithNonMatchingConstStrings_ThenFaseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal("hallo", "hej")
				);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithNullSubject_ThenFaseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal((char*) NULL, "yes?")
				);
			}

			TEST_METHOD(WhenStrings_Are_EqualIsCalled_WithNullExpected_ThenFaseIsReturned)
			{
				Assert::AreEqual(
					false,
					Strings_Are_Equal("yo", (char*)NULL)
				);
			}

			TEST_METHOD(WhenConvert_String_To_Upper_CaseIsCalled_WithNonBlankString_ThenUppercaseStringIsReturned)
			{
				auto testStr = strdup("loWerCasisH");

				Convert_String_To_Upper_Case(testStr);

				Assert::AreEqual("LOWERCASISH", testStr);
			}

			TEST_METHOD(WhenConvert_String_To_Upper_CaseIsCalled_WithNonBlankConstString_ThenUppercaseStringIsReturned)
			{
				auto testStr = "loWerDeasisH";

				auto result = Convert_String_To_Upper_Case(testStr);

				Assert::AreEqual("LOWERDEASISH", result);
			}
	};
}
