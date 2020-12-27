#include "CppUnitTest.h"

#include <utils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	namespace Nco
	{
		namespace Common
		{
			TEST_CLASS(UtilsTests)
			{
			public:
				TEST_METHOD(When_IsIntString_IsCalled_WithConstStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsIntString("938")
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithConstStringContainingAnSingleDigit_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsIntString("1")
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithConstStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString("null")
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithNonConstStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsIntString(strdup("938"))
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithNonConstStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString(strdup("null"))
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithConstNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString((const char*)NULL)
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithConstEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString("")
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithNonConstNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString((char*)NULL)
					);
				}

				TEST_METHOD(When_IsIntString_IsCalled_WithNonConstEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsIntString(strdup(""))
					);
				}

				TEST_METHOD(When_IsDoubleString_IsCalled_WithStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsDoubleString("39")
					);
				}

				TEST_METHOD(When_IsDoubleString_IsCalled_WithStringContainingADouble_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsDoubleString("837.38")
					);
				}

				TEST_METHOD(When_IsDoubleString_IsCalled_WithStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsDoubleString("kane lives!")
					);
				}

				TEST_METHOD(When_IsDoubleString_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsDoubleString((char*)NULL)
					);
				}

				TEST_METHOD(When_IsDoubleString_IsCalled_WithEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsDoubleString("")
					);
				}

				TEST_METHOD(When_ParseNumber_IsCalled_WithNumberChar_ThenNumberIsReturned)
				{
					Assert::AreEqual(
						9,
						ParseNumber('9')
					);
				}

				TEST_METHOD(When_ConvertNumberToString_IsCalled_WithMinInt_ThenStringIsReturned)
				{
					Assert::AreEqual(
						"-2147483648",
						ConvertNumberToString(INT_MIN)
					);
				}

				TEST_METHOD(When_ConvertNumberToString_IsCalled_WithZero_ThenStringIsReturned)
				{
					Assert::AreEqual(
						"0",
						ConvertNumberToString(0)
					);
				}

				TEST_METHOD(When_ConvertNumberToString_IsCalled_WithIntMax_ThenStringIsReturned)
				{
					Assert::AreEqual(
						"2147483647",
						ConvertNumberToString(INT_MAX)
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithUppercaseStringContainingTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsBooleanString("TRUE")
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithLoweraseStringContainingTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsBooleanString("true")
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithUppercaseStringContainingFalse_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsBooleanString("FALSE")
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithLoweraseStringContainingFalse_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						IsBooleanString("false")
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsBooleanString(NULL)
					);
				}

				TEST_METHOD(When_IsBooleanString_IsCalled_WithBlankString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						IsBooleanString("")
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithUppercaseStringContainingTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						ParseBoolean("TRUE", NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithUppercaseStringContainingTrue_ThenParseErrorIsNotReturned)
				{
					bool parseError = false;

					ParseBoolean("TRUE", &parseError);

					Assert::AreEqual(false, parseError);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithLowercaseStringContainingTrue_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBoolean("true", NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithLowercaseStringContainingTrue_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					ParseBoolean("true", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithUppercaseStringContainingFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBoolean("FALSE", NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithUppercaseStringContainingFalse_ThenParseErrorIsNotReturned)
				{
					bool parseError = false;

					ParseBoolean("FALSE", &parseError);

					Assert::AreEqual(false, parseError);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithLowercaseStringContainingFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBoolean("false", NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithLowercaseStringContainingFalse_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					ParseBoolean("false", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBoolean((char*)NULL, NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithNullString_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					ParseBoolean((char*)NULL, &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBoolean("", NULL)
					);
				}

				TEST_METHOD(When_ParseBoolean_IsCalled_WithEmptyString_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					ParseBoolean("", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithUppercaseStringContainingTrueAndDefaultOfFalse_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						ParseBooleanOrDefault("TRUE", false)
					);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithLowercaseStringContainingTrueAndDefaultOfFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBooleanOrDefault("true", false)
					);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithUppercaseStringContainingTrueAndDefaultOfTrue_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						ParseBooleanOrDefault("FALSE", true)
					);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithLowercaseStringContainingFalseAndDefaultOfTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						ParseBooleanOrDefault("false", true)
					);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithNullString_ThenDefaultIsReturned)
				{
					Assert::AreEqual(
						true,
						ParseBooleanOrDefault((char*)NULL, true)
					);
				}

				TEST_METHOD(When_ParseBooleanOrDefault_IsCalled_WithEmptyString_ThenDefaultIsReturned)
				{
					Assert::AreEqual(
						true,
						ParseBooleanOrDefault("", true)
					);
				}

				TEST_METHOD(When_ConvertBooleanToString_IsCalled_WithFalse_ThenFalseStringIsReturned)
				{
					Assert::AreEqual(
						"FALSE",
						ConvertBooleanToString(false)
					);
				}

				TEST_METHOD(When_ConvertBooleanToString_IsCalled_WithTrue_ThenTrueStringIsReturned)
				{
					Assert::AreEqual(
						"TRUE",
						ConvertBooleanToString(true)
					);
				}

			};
		}
	}
}
