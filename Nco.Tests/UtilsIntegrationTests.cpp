#include "CppUnitTest.h"

#include <shlwapi.h>

#include <strings.h>
#include <utils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static auto TEST_FILENAME = "test_file.txt";

namespace Integration
{
	namespace Nco
	{
		namespace Common
		{
			TEST_CLASS(UtilsIntegrationTests)
			{
				HANDLE fileHandle;

			public:
				TEST_METHOD_INITIALIZE(Setup)
				{
					if (PathFileExists(TEST_FILENAME))
					{
						DeleteFile(TEST_FILENAME);
					}
				}

				TEST_METHOD_CLEANUP(TearDown)
				{
					if (fileHandle != NULL && fileHandle != INVALID_HANDLE_VALUE)
					{
						CloseHandle(fileHandle);
						fileHandle = NULL;
					}
				}

				TEST_METHOD(When_Get_Env_Var_IsCalled_WithPresentVarName_ThenValueIsReturned)
				{
					Assert::AreEqual(
						"Windows_NT",
						Get_Env_Var("OS", NULL)
					);
				}

				TEST_METHOD(When_Get_Env_Var_IsCalled_WithPresentVarName_ThenValueFoundIsSetToTrue)
				{
					bool valueFound = false;

					Get_Env_Var("OS", &valueFound);

					Assert::IsTrue(valueFound);
				}

				TEST_METHOD(When_Get_Env_Var_Or_WithMissingVarName_ThenValueFoundIsSetToFalse)
				{
					bool valueFound = true;

					Get_Env_Var("CARGO_SIZE", &valueFound);

					Assert::IsFalse(valueFound);
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
	}
}
