#include <vector>

#include <CppUnitTest.h>
#include <lua.hpp>

#include <ILuaStateWrapper.h>
#include <LuaStateWrapper.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit
{
	namespace Nco
	{
		namespace Lua
		{
			TEST_CLASS(LuaStateWrapperTests)
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

					luaState = NULL;

					delete luaWrapper;
				}

				TEST_METHOD(When_ExecuteScript_IsCalled_And_ScriptDoesNotError_Then_ErrorResultIsNotReturned)
				{
					auto& result = luaWrapper->ExecuteScript("local output = 'Mission Accomplished'");

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ExecuteScript_IsCalled_And_ScriptDoesError_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ExecuteScript("iDontExist.wweeeeeee('are').dumb()");

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ExecuteScript_IsCalled_And_ScriptDoesError_Then_ErrorResultContainsMessage)
				{
					auto& result = luaWrapper->ExecuteScript("iDontExist.wweeeeeee()");

					Assert::AreEqual("[string \"iDontExist.wweeeeeee()\"]:1: attempt to index a nil value (global 'iDontExist')", result.GetError());
				}

				TEST_METHOD(When_ReadInt_IsCalled_And_AnIntIsOnTheStack_Then_IntValueIsReturned)
				{
					lua_pushinteger(luaState, 1);

					auto val = luaWrapper->ReadInteger(1).GetValue();

					Assert::AreEqual(1, val);
				}

				TEST_METHOD(When_ReadInt_IsCalled_And_AnIntIsOnTheStack_Then_ErrorResultIsNotReturned)
				{
					lua_pushinteger(luaState, 1);

					auto& result = luaWrapper->ReadInteger(1);

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadInt_IsCalled_And_AnIntIsNotOnTheStack_Then_DefaultValueIsReturned)
				{
					auto val = luaWrapper->ReadInteger(1).GetValue();

					Assert::AreEqual(0, val);
				}

				TEST_METHOD(When_ReadInt_IsCalled_And_AnIntIsNotOnTheStack_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ReadInteger(1);

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadDouble_IsCalled_And_ADoubleIsOnTheStack_Then_DoubleValueIsReturned)
				{
					lua_pushnumber(luaState, 3.52);

					auto val = luaWrapper->ReadDouble(1).GetValue();

					Assert::AreEqual(3.52, val);
				}

				TEST_METHOD(When_ReadDouble_IsCalled_And_ADoubleIsOnTheStack_Then_ErrorResultIsNotReturned)
				{
					lua_pushnumber(luaState, 3.52);

					auto& result = luaWrapper->ReadDouble(1);

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadDouble_IsCalled_And_ADoubleIsNotOnTheStack_Then_DefaultValueIsReturned)
				{
					auto val = luaWrapper->ReadDouble(1).GetValue();

					Assert::AreEqual(0.0, val);
				}

				TEST_METHOD(When_ReadDouble_IsCalled_And_ADoubleIsNotOnTheStack_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ReadDouble(1);

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadBool_IsCalled_And_ABoolIsOnTheStack_Then_BoolValueIsReturned)
				{
					lua_pushboolean(luaState, true);

					auto val = luaWrapper->ReadBool(1).GetValue();

					Assert::AreEqual(true, val);
				}

				TEST_METHOD(When_ReadBool_IsCalled_And_ABoolIsOnTheStack_Then_ErrorResultIsNotReturned)
				{
					lua_pushboolean(luaState, true);

					auto& result = luaWrapper->ReadBool(1);

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadBool_IsCalled_And_ABoolIsNotOnTheStack_Then_DefaultValueIsReturned)
				{
					auto val = luaWrapper->ReadBool(1).GetValue();

					Assert::AreEqual(false, val);
				}

				TEST_METHOD(When_ReadBool_IsCalled_And_ABoolIsNotOnTheStack_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ReadBool(1);

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadString_IsCalled_And_AStringIsOnTheStack_Then_StringValueIsReturned)
				{
					lua_pushstring(luaState, "hey");

					auto val = luaWrapper->ReadString(1).GetValue();

					Assert::AreEqual("hey", val);
				}

				TEST_METHOD(When_ReadString_IsCalled_And_AStringIsOnTheStack_Then_ErrorResultIsNotReturned)
				{
					lua_pushstring(luaState, "hey");

					auto& result = luaWrapper->ReadString(1);

					Assert::AreEqual(false, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadString_IsCalled_And_AStringIsNotOnTheStack_Then_DefaultValueIsReturned)
				{
					auto val = luaWrapper->ReadString(1).GetValue();

					Assert::AreEqual(NULL, val);
				}

				TEST_METHOD(When_ReadString_IsCalled_And_AStringIsNotOnTheStack_Then_ErrorResultIsReturned)
				{
					auto& result = luaWrapper->ReadString(1);

					Assert::AreEqual(true, result.IsErrorResult());
				}

				TEST_METHOD(When_ReadArray_IsCalled_Then_ArrayIsReadFromLua)
				{
					lua_createtable(luaState, 0, 2);

					lua_pushnumber(luaState, 3.42);
					lua_rawseti(luaState, -2, 1);
					lua_pushnumber(luaState, 45.9);
					lua_rawseti(luaState, -2, 2);

					auto& arrayOut = luaWrapper->ReadArray<double>();

					Assert::AreEqual((size_t)2, arrayOut.size());

					Assert::AreEqual(3.42, arrayOut[0]);
					Assert::AreEqual(45.9, arrayOut[1]);
				}

				TEST_METHOD(When_GetLastError_IsCalled_Then_ErrorMessageIsReturned)
				{
					luaL_dostring(luaState, "££a$$ = £R£RRTabv:00");

					auto message = luaWrapper->GetLastError();

					Assert::AreEqual("[string \"££a$$ = £R£RRTabv:00\"]:1: unexpected symbol near '<\\163>'", message);
				}

				TEST_METHOD(When_WriteInt_IsCalled_Then_IntValueIsPushedOntoTheStack)
				{
					luaWrapper->WriteInteger(1);

					int val = luaL_checkinteger(luaState, 1);

					Assert::AreEqual(1, val);
				}

				TEST_METHOD(When_WriteDouble_IsCalled_Then_DoubleValueIsPushedOntoTheStack)
				{
					luaWrapper->WriteDouble(45.29);

					double val = lua_tonumber(luaState, 1);

					Assert::AreEqual(45.29, val);
				}

				TEST_METHOD(When_WriteBool_IsCalled_Then_BoolValueIsPushedOntoTheStack)
				{
					luaWrapper->WriteBool(true);

					bool val = lua_toboolean(luaState, 1);

					Assert::AreEqual(true, val);
				}

				TEST_METHOD(When_WriteString_IsCalled_Then_StringValueIsPushedOntoTheStack)
				{
					luaWrapper->WriteString("Over and out");

					const char* val = lua_tostring(luaState, 1);

					Assert::AreEqual("Over and out", val);
				}

				TEST_METHOD(When_WriteArray_IsCalled_WithVector_Then_ArrayTableIsPushedOntoTheStack)
				{
					auto& table = *new std::vector<int>{ 3, 234, 156 };

					luaWrapper->WriteArray(table);

					Assert::AreEqual(true, lua_istable(luaState, lua_gettop(luaState)));
					Assert::AreEqual(3, (int)lua_rawlen(luaState, lua_gettop(luaState)));

					lua_pushvalue(luaState, lua_gettop(luaState));
					lua_pushnil(luaState);

					while (lua_next(luaState, -2))
					{
						lua_pushvalue(luaState, -2);

						auto idx = lua_tointeger(luaState, -1);
						auto value = lua_tointeger(luaState, -2);

						Assert::AreEqual(table[idx - 1], (int)value);

						lua_pop(luaState, 2);
					}
				}
			};
		}
	}
}
