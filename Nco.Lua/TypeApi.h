#pragma once

#include <functional>

#include <Logger.h>
#include <Optional.h>
#include <Strings.h>

#include "LuaApi.h"
#include "LuaObjectUtils.h"
#include <Result.h>
#include "LuaStateWrapper.h"
#include "TypeApiParameters.h"

template<class T> class TypeApi : public LuaApi
{
private:
	static int ReadRuleValueApiProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(TypeApi, ReadRuleLua);
	}

	static int WriteRuleValueApiProxy(lua_State* lua)
	{
		return LUA_METHOD_PROXY(TypeApi, WriteRuleLua);
	}

	TypeApiParameters<T>& GetParameters(const char* operation, ILuaStateWrapper& luaState)
	{
		LogTrace("%s%sRule called from Lua", operation, titleCaseTypeName);

		int argCount = luaState.GetStackTop();

		if (argCount < 2)
		{
			luaState.RaiseError("%s%sRule requires exactly two arguments", operation, titleCaseTypeName);

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& typeInstanceNameResult = luaState.ReadString(1);

		if (typeInstanceNameResult.IsErrorResult() || StringIsEmpty(typeInstanceNameResult.GetValue()))
		{
			luaState.RaiseError("%s%sRule argument `typeName` was nil or blank", operation, titleCaseTypeName);

			delete &typeInstanceNameResult;

			return TypeApiParameters<T>::BuildInvalid();
		}

		if (!ValidateTypeName(typeInstanceNameResult.GetValue()))
		{
			delete &typeInstanceNameResult;

			luaState.RaiseError("%s%sRule argument `typeName` was not recognised as a valid type", operation, titleCaseTypeName);

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& typeInstance = ParseType(typeInstanceNameResult.GetValue());
		auto& ruleNameParameterResult = luaState.ReadString(2);

		if (ruleNameParameterResult.IsErrorResult() || StringIsEmpty(ruleNameParameterResult.GetValue()))
		{
			luaState.RaiseError("%s%sRule argument `ruleName` was nil", operation, titleCaseTypeName);

			delete &typeInstanceNameResult;
			delete &ruleNameParameterResult;

			return TypeApiParameters<T>::BuildInvalid();
		}

		if (!ValidateRule(ruleNameParameterResult.GetValue()))
		{
			luaState.RaiseError(
				"rule name '%s' passed to %sRule was not recognised for this type: %s",
				ruleNameParameterResult.GetValue(),
				operation,
				titleCaseTypeName
			);

			delete &typeInstanceNameResult;
			delete &ruleNameParameterResult;

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& params = TypeApiParameters<T>::BuildValid(
			typeInstanceNameResult.GetValue(),
			ruleNameParameterResult.GetValue(),
			&typeInstance
		);

		delete &typeInstanceNameResult;
		delete &ruleNameParameterResult;

		return params;
	}

	int ReadRuleLua(ILuaStateWrapper& luaState)
	{
		auto& params = GetParameters("get", luaState);

		if (!params.IsValid())
		{
			delete &params;

			return 0;
		}

		LogDebug("get%sRule => attempting to read value of rule '%s'", titleCaseTypeName, params.ruleName);

		auto& readResult = ReadRule(luaState, *params.typeInstance, params.ruleName);

		if (!readResult.IsErrorResult())
		{
			LogDebug("get%sRule => Read rule success", titleCaseTypeName);

			delete &params;
			delete &readResult;

			return 1;
		}

		luaState.RaiseError("Read rule error for type '%s': %s", titleCaseTypeName, readResult.GetError());

		delete &params;
		delete &readResult;

		return 0;
	}

	int WriteRuleLua(ILuaStateWrapper& luaState)
	{
		auto& params = GetParameters("set", luaState);

		if (!params.IsValid())
		{
			delete &params;

			return 0;
		}

		LogDebug("set%sRule => attempting to write value of rule '%s'", titleCaseTypeName, params.ruleName);

		auto& writeResult = WriteRule(luaState, *params.typeInstance, params.ruleName, 3);

		if (!writeResult.IsErrorResult())
		{
			LogDebug("set%sRule => Write rule success", titleCaseTypeName);

			delete &params;
			delete &writeResult;

			return 1;
		}

		luaState.RaiseError("Write rule error for type '%s': %s", titleCaseTypeName, writeResult.GetError());

		delete &params;
		delete &writeResult;

		return 0;
	}

protected:
	const char* typeName;
	const char* titleCaseTypeName;

	TypeApi(char* typeName) : typeName(typeName), titleCaseTypeName(ToTitleCase(typeName))
	{
		WithName(typeName);
		WithDescription(FormatString("%s rule info and control functions", titleCaseTypeName));

		auto name = titleCaseTypeName;

		WithMethod("getRule", this, ReadRuleValueApiProxy)
			.WithDescription(FormatString("Set a rule for a given %s", titleCaseTypeName))
			.WithParameter("ruleName", [](LuaVariableInfo& vi) {
				vi.WithDescription("The name as it appears in RULES.INI")
				  .WithType(LuaType::String);
			})
			.WithReturnValue("ruleValue", [](LuaVariableInfo& vi) {
				vi.WithDescription("The current value for the specified rule")
				  .WithType(LuaType::Any);
			});
		

		WithMethod("setRule", this, WriteRuleValueApiProxy)
			.WithDescription(FormatString("Get a rule for a given", titleCaseTypeName))
			.WithParameter("ruleName", [](LuaVariableInfo& vi) {
				vi.WithDescription("The name as it appears in RULES.INI")
				  .WithType(LuaType::String);
			})
			.WithParameter("ruleValue", [](LuaVariableInfo& vi) {
				vi.WithDescription("A valid value for the specified rule")
			      .WithType(LuaType::Any);
			});
	}

	virtual bool ValidateRule(const char* ruleName) = 0;

	virtual bool ValidateTypeName(const char* typeName) = 0;

	virtual T& ParseType(const char* typeName) = 0;

	virtual Result& ReadRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName) = 0;

	virtual Result& WriteRule(ILuaStateWrapper& lua, T& typeInstance, const char* ruleName, int ruleValueStackIndex) = 0;

public:
	~TypeApi()
	{
		if (typeName != NULL)
		{
			delete typeName;
		}

		if (titleCaseTypeName != NULL)
		{
			delete titleCaseTypeName;
		}
	}

};
