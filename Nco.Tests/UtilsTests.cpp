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
				TEST_METHOD(When_Is_Int_String_IsCalled_WithConstStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Int_String("938")
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithConstStringContainingAnSingleDigit_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Int_String("1")
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithConstStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String("null")
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithNonConstStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Int_String(strdup("938"))
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithNonConstStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String(strdup("null"))
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithConstNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String((const char*)NULL)
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithConstEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String("")
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithNonConstNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String((char*)NULL)
					);
				}

				TEST_METHOD(When_Is_Int_String_IsCalled_WithNonConstEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Int_String(strdup(""))
					);
				}

				TEST_METHOD(When_Is_Double_String_IsCalled_WithStringContainingAnInt_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Double_String("39")
					);
				}

				TEST_METHOD(When_Is_Double_String_IsCalled_WithStringContainingADouble_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Double_String("837.38")
					);
				}

				TEST_METHOD(When_Is_Double_String_IsCalled_WithStringContainingNoNumbers_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Double_String("kane lives!")
					);
				}

				TEST_METHOD(When_Is_Double_String_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Double_String((char*)NULL)
					);
				}

				TEST_METHOD(When_Is_Double_String_IsCalled_WithEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Double_String("")
					);
				}

				TEST_METHOD(When_Parse_Number_IsCalled_WithNumberChar_ThenNumberIsReturned)
				{
					Assert::AreEqual(
						9,
						Parse_Number('9')
					);
				}

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

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithUppercaseStringContainingTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Boolean_String("TRUE")
					);
				}

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithLoweraseStringContainingTrue_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Boolean_String("true")
					);
				}

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithUppercaseStringContainingFalse_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Is_Boolean_String("FALSE")
					);
				}

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithLoweraseStringContainingFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Boolean_String("false")
					);
				}

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Boolean_String(NULL)
					);
				}

				TEST_METHOD(When_Is_Boolean_String_IsCalled_WithBlankString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Is_Boolean_String("")
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithUppercaseStringContainingTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Parse_Boolean("TRUE", NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithUppercaseStringContainingTrue_ThenParseErrorIsNotReturned)
				{
					bool parseError = false;

					Parse_Boolean("TRUE", &parseError);

					Assert::AreEqual(false, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithLowercaseStringContainingTrue_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean("true", NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithLowercaseStringContainingTrue_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					Parse_Boolean("true", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithUppercaseStringContainingFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean("FALSE", NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithUppercaseStringContainingFalse_ThenParseErrorIsNotReturned)
				{
					bool parseError = false;

					Parse_Boolean("FALSE", &parseError);

					Assert::AreEqual(false, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithLowercaseStringContainingFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean("false", NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithLowercaseStringContainingFalse_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					Parse_Boolean("false", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithNullString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean((char*)NULL, NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithNullString_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					Parse_Boolean((char*)NULL, &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithEmptyString_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean("", NULL)
					);
				}

				TEST_METHOD(When_Parse_Boolean_IsCalled_WithEmptyString_ThenParseErrorIsReturned)
				{
					bool parseError = false;

					Parse_Boolean("", &parseError);

					Assert::AreEqual(true, parseError);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithUppercaseStringContainingTrueAndDefaultOfFalse_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Parse_Boolean_Or_Default("TRUE", false)
					);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithLowercaseStringContainingTrueAndDefaultOfFalse_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean_Or_Default("true", false)
					);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithUppercaseStringContainingTrueAndDefaultOfTrue_ThenFalseIsReturned)
				{
					Assert::AreEqual(
						false,
						Parse_Boolean_Or_Default("FALSE", true)
					);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithLowercaseStringContainingFalseAndDefaultOfTrue_ThenTrueIsReturned)
				{
					Assert::AreEqual(
						true,
						Parse_Boolean_Or_Default("false", true)
					);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithNullString_ThenDefaultIsReturned)
				{
					Assert::AreEqual(
						true,
						Parse_Boolean_Or_Default((char*)NULL, true)
					);
				}

				TEST_METHOD(When_Parse_Boolean_Or_Default_IsCalled_WithEmptyString_ThenDefaultIsReturned)
				{
					Assert::AreEqual(
						true,
						Parse_Boolean_Or_Default("", true)
					);
				}

				TEST_METHOD(When_Convert_Boolean_To_String_IsCalled_WithFalse_ThenFalseStringIsReturned)
				{
					Assert::AreEqual(
						"FALSE",
						Convert_Boolean_To_String(false)
					);
				}

				TEST_METHOD(When_Convert_Boolean_To_String_IsCalled_WithTrue_ThenTrueStringIsReturned)
				{
					Assert::AreEqual(
						"TRUE",
						Convert_Boolean_To_String(true)
					);
				}
			};
		}
	}
}
