#include <CppUnitTest.h>
#include <lua.hpp>

#include <ILuaStateWrapper.h>
#include <ILuaRuntime.h>
#include <LuaRuntime.h>
#include <LuaStateWrapper.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Integration
{
	namespace Nco
	{
		namespace Lua
		{
			TEST_CLASS(LuaRuntimeIntegrationTests)
			{
			private:
				lua_State* luaState;
				ILuaStateWrapper* luaWrapper;
				ILuaRuntime* luaRuntime;

				void RegisterTestApi()
				{
					luaRuntime->RegisterApi("testy", [](ILuaApi& a) {
						a.WithDescription("This is a test Lua API!")
						 .WithFunction(
							"addNumbers",
							[](lua_State* l) {
								 lua_pushnumber(l,
									 luaL_checknumber(l, 1) + luaL_checknumber(l, 2)
								 );

								 return 1;
							},
							[](LuaFunctionInfo& f) {
								f.WithDescription("Adds two numbers together")
								 .WithParameter("x", [](LuaVariableInfo& v) {
									v.WithType(LuaType::Number)
									 .WithDescription("Left hand value");
								 })
								 .WithParameter("y", [](LuaVariableInfo& v) {
									v.WithType(LuaType::Number)
									 .WithDescription("Right hand value");
								 })
								 .WithReturnValue("sum", [](LuaVariableInfo& v) {
									v.WithType(LuaType::Number)
									 .WithDescription("The sum of x and y");
								 });
							}
						);
					});
				}

			public:
				TEST_METHOD_INITIALIZE(Setup)
				{
					luaState = luaL_newstate();

					luaWrapper = &LuaStateWrapper::Build(luaState);
					luaRuntime = &LuaRuntime::Build(*luaWrapper);
				}

				TEST_METHOD_CLEANUP(TearDown)
				{
					lua_close(luaState);
					delete luaWrapper;
				}
				
				TEST_METHOD(When_RegisterApi_IsCalled_ThenGetApisReturnsNewApi)
				{
					RegisterTestApi();

					auto& apis = luaRuntime->GetApis();

					Assert::AreEqual((size_t)1, apis.size());
					Assert::AreEqual("testy", apis[0]->GetName());
				}

				TEST_METHOD(When_RegisterApi_IsCalled_ThenApiFunctionCanBeCalledFromLua)
				{
					RegisterTestApi();

					luaWrapper->ExecuteScript("return addNumbers(4365.513, 82)");

					auto sumResult = luaWrapper->ReadDouble();

					Assert::AreEqual(false, sumResult.IsErrorResult());
					Assert::AreEqual(4447.5129999999999, sumResult.GetValue());
				}
			};
		}
	}
}
