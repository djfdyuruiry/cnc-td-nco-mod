#include "pch.h"
#include "CppUnitTest.h"

#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	TEST_CLASS(UtilsTests)
	{
		public:
			TEST_METHOD(When_Convert_Number_To_String_IsCalled_WithMinInt_ThenStringIsReturned)
			{
				Assert::AreEqual(
					"-2147483648",
					Convert_Number_To_String(INT_MIN)
				);
			}

			TEST_METHOD(When_Convert_Number_To_String_IsCalled_WithZero_ThenStringIsReturned)
			{
				Assert::AreEqual(
					"0",
					Convert_Number_To_String(0)
				);
			}

			TEST_METHOD(When_Convert_Number_To_String_IsCalled_WithIntMax_ThenStringIsReturned)
			{
				Assert::AreEqual(
					"2147483647",
					Convert_Number_To_String(INT_MAX)
				);
			}
	};
}
