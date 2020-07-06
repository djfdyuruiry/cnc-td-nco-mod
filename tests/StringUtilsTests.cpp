#include "pch.h"
#include "CppUnitTest.h"

#include "strings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	TEST_CLASS(StringUtilsTests)
	{
	public:
		TEST_METHOD(WhenConvert_String_To_Upper_CaseIsCalled_WithNonBlankString_ThenUppercaseStringIsReturned)
		{
			auto testStr = strdup("loWerCasisH");

			Convert_String_To_Upper_Case(testStr);

			Assert::AreEqual(testStr, "LOWERCASISH");
		}

		TEST_METHOD(WhenConvert_String_To_Upper_CaseIsCalled_WithNonBlankConstString_ThenUppercaseStringIsReturned)
		{
			auto testStr = "loWerDeasisH";

			auto result = Convert_String_To_Upper_Case(testStr);

			Assert::AreEqual(result, "LOWERDEASISH");
		}
	};
}
