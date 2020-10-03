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
					luaRuntime->RegisterApi("testy", [](ILuaApi& a) {
						a.WithDescription("This is a test Lua API!")
						 .WithFunction(
							"addNumbers",
							[](lua_State* s) {
								 return 0;
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
							});
					});

					auto& apis = luaRuntime->GetApis();

					Assert::AreEqual((size_t)1, apis.size());
					Assert::AreEqual("testy", apis[0]->GetName());
				}
			};
		}
	}
}
