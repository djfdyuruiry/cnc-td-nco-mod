#include "pch.h"
#include "CppUnitTest.h"

#include "strings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TiberianDawn
{
	TEST_CLASS(StringsTests)
	{
	public:
		TEST_METHOD(WhenConvert_String_To_Upper_CaseIsCalled_WithNonBlankString_ThenUppercaseStringIsReturned)
		{
			auto testStr = "loWerCasisH";

			auto result = Convert_String_To_Upper_Case(testStr);

			Assert::AreEqual(result, "LOWERCASISH");
		}
	};
}
