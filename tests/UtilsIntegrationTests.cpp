#include "CppUnitTest.h"

#include <shlwapi.h>

#include "strings.h"
#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const auto TEST_FILENAME = "test_file.txt";
static HANDLE fileHandle;

namespace Integration
{
	TEST_CLASS(UtilsIntegrationTests)
	{
		public:
			UtilsIntegrationTests()
			{
				if (fileHandle != NULL && fileHandle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(fileHandle);
					fileHandle = NULL;
				}

				if (PathFileExists(TEST_FILENAME))
				{
					DeleteFile(TEST_FILENAME);
				}
			}

			TEST_METHOD(When_Get_Env_Var_Or_Default_IsCalled_WithPresentVarName_ThenValueIsReturned)
			{
				Assert::AreEqual(
					"Windows_NT",
					Get_Env_Var_Or_Default("OS", "Ubuntu 20.04 LTS")
				);
			}

			TEST_METHOD(When_Get_Env_Var_Or_Default_IsCalled_WithMissingVarName_ThenDefaultIsReturned)
			{
				Assert::AreEqual(
					"4200KG",
					Get_Env_Var_Or_Default("CARGO_SIZE", "4200KG")
				);
			}

			TEST_METHOD(When_Open_File_For_Appending_IsCalled_WithValidNewFileName_ThenNoErrorIsReturned)
			{
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				Assert::AreEqual(
					false,
					errorOccurred
				);
			}

			TEST_METHOD(When_Open_File_For_Appending_IsCalled_WithValidNewFileName_ThenHandleIsReturned)
			{
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				Assert::AreNotEqual(
					INVALID_HANDLE_VALUE,
					fileHandle
				);
			}

			TEST_METHOD(When_Open_File_For_Appending_IsCalled_WithValidExistingFileName_ThenNoErrorIsReturned)
			{
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);
				CloseHandle(fileHandle);

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				Assert::AreEqual(
					false,
					errorOccurred
				);
			}

			TEST_METHOD(When_Open_File_For_Appending_IsCalled_WithValidExistingFileName_ThenHandleIsReturned)
			{
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				CloseHandle(fileHandle);

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				Assert::AreNotEqual(
					INVALID_HANDLE_VALUE,
					fileHandle
				);
			}

			TEST_METHOD(When_Open_File_For_Appending_IsCalled_WithInvalidFileName_ThenErrorIsReturned)
			{
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending("Y:\\somewhere\\test_file.txt", &errorOccurred);

				Assert::AreEqual(
					true,
					errorOccurred
				);
			}

			TEST_METHOD(When_Append_To_File_IsCalled_WithValidHandle_ThenStringIsWrittenToFile)
			{
				char* expected = "Hello there!";

				// append
				bool errorOccurred = false;

				fileHandle = Open_File_For_Appending(TEST_FILENAME, &errorOccurred);

				Append_To_File(fileHandle, expected);

				CloseHandle(fileHandle);

				// read
				auto readBuffer = Allocate_String(strlen(expected));

				fileHandle = CreateFile(
					TEST_FILENAME,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);
	
				ReadFile(
					fileHandle,
					readBuffer,
					13,
					NULL,
					NULL
				);

				Assert::AreEqual(
					"Hello there!",
					readBuffer
				);
			}
	};
}
