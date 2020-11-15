#include <CppUnitTest.h>
#include <lua.hpp>

#include <ILuaStateWrapper.h>
#include <LuaStateWrapper.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Integration
{
	namespace Nco
	{
		namespace Lua
		{
			TEST_CLASS(LuaStateWrapperIntegrationTests)
			{
			private:
				lua_State* luaState;
				ILuaStateWrapper* luaWrapper;

			public:
				TEST_METHOD_INITIALIZE(Setup)
				{
					luaState = luaL_newstate();

					luaWrapper = &LuaStateWrapper::Build(luaState);
				}

				TEST_METHOD_CLEANUP(TearDown)
				{
					lua_close(luaState);
					delete luaWrapper;
				}

				TEST_METHOD(When_ExecuteFile_IsCalled_And_ScriptDoesNotError_Then_ErrorResultIsNotReturned)
				{
					auto& result = luaWrapper->ExecuteFile("../../ok-script.lua");

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ExecuteFile_IsCalled_And_ScriptDoesError_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ExecuteFile("../../error-script.lua");

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ExecuteFile_IsCalled_And_ScriptDoesError_Then_ErrorResultContainsMessage)
				{
					auto& result = luaWrapper->ExecuteFile("../../error-script.lua");

					Assert::AreEqual("../../error-script.lua:1: attempt to index a nil value (global 'Thanos')", result.GetError());
				}

				TEST_METHOD(When_ExecuteFile_IsCalled_And_FileIsMissing_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ExecuteFile("K:\\wat.lua");

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ExecuteFile_IsCalled_And_FileIsMissing_Then_ErrorResultContainsMessage)
				{
					auto& result = luaWrapper->ExecuteFile("K:\\wat.lua");

					Assert::AreEqual("cannot open K:\\wat.lua: No such file or directory", result.GetError());
				}

			};
		}
	}
}
