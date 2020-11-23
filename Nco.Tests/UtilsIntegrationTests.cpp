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

				TEST_METHOD(When_GetEnvVar_IsCalled_WithPresentVarName_ThenValueIsReturned)
				{
					Assert::AreEqual(
						"Windows_NT",
						GetEnvVar("OS", NULL)
					);
				}

				TEST_METHOD(When_GetEnvVar_IsCalled_WithPresentVarName_ThenValueFoundIsSetToTrue)
				{
					bool valueFound = false;

					GetEnvVar("OS", &valueFound);

					Assert::IsTrue(valueFound);
				}

				TEST_METHOD(When_GetEnvVar_Or_WithMissingVarName_ThenValueFoundIsSetToFalse)
				{
					bool valueFound = true;

					GetEnvVar("CARGO_SIZE", &valueFound);

					Assert::IsFalse(valueFound);
				}
			};
		}
	}

}
