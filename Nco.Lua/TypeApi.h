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
		LogTrace("%sRule called from Lua", operation);

		int argCount = luaState.GetStackTop();

		if (argCount < 2)
		{
			luaState.RaiseError("%sRule requires exactly two arguments", operation);

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& typeInstanceNameResult = luaState.ReadString(1);

		if (typeInstanceNameResult.IsErrorResult() || StringIsEmpty(typeInstanceNameResult.GetValue()))
		{
			luaState.RaiseError("%sRule argument `typeName` was nil or blank", operation);

			delete &typeInstanceNameResult;

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto typeInstanceName = typeInstanceNameResult.GetValue();

		delete &typeInstanceNameResult;

		if (!ValidateTypeName(typeInstanceName))
		{
			luaState.RaiseError(
				"%sRule argument `typeName` was not recognised as a valid '%s' type: %s",
				operation,
				titleCaseTypeName,
				typeInstanceName
			);

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& typeInstance = ParseType(typeInstanceName);
		auto& ruleNameParameterResult = luaState.ReadString(2);

		if (ruleNameParameterResult.IsErrorResult() || StringIsEmpty(ruleNameParameterResult.GetValue()))
		{
			luaState.RaiseError("%sRule argument `ruleName` was nil or blank", operation);

			delete &ruleNameParameterResult;

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto ruleParameterName = ruleNameParameterResult.GetValue();

		delete &ruleNameParameterResult;

		if (!ValidateRule(ruleParameterName))
		{
			luaState.RaiseError(
				"rule passed to %sRule was not recognised for type '%s': %s",
				operation,
				titleCaseTypeName,
				ruleParameterName
			);

			return TypeApiParameters<T>::BuildInvalid();
		}

		auto& params = TypeApiParameters<T>::BuildValid(
			typeInstanceName,
			ruleParameterName,
			&typeInstance
		);

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

		LogDebug(
			"getRule => attempting to read value of rule for type '%s': %s -> %s",
			titleCaseTypeName,
			params.typeInstanceName,
			params.ruleName
		);

		auto& readResult = ReadRule(luaState, *params.typeInstance, params.ruleName);

		if (!readResult.IsErrorResult())
		{
			LogDebug(
				"getRule => Read rule success for type '%s': %s -> %s",
				titleCaseTypeName,
				params.typeInstanceName,
				params.ruleName
			);

			delete &params;
			delete &readResult;

			return 1;
		}

		luaState.RaiseError(
			"Read rule '%s -> %s' error for type '%s': %s",
			params.typeInstanceName,
			params.ruleName,
			titleCaseTypeName,
			readResult.GetError()
		);

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

		LogDebug(
			"setRule => attempting to write value of rule for type '%s': %s -> %s",
			titleCaseTypeName,
			params.typeInstanceName,
			params.ruleName
		);

		auto& writeResult = WriteRule(luaState, *params.typeInstance, params.ruleName, 3);

		if (!writeResult.IsErrorResult())
		{
			LogDebug("setRule => Write rule success for type: %s", titleCaseTypeName);

			delete &params;
			delete &writeResult;

			return 1;
		}

		luaState.RaiseError(
			"Write rule '%s -> %s' error for type '%s': %s",
			params.typeInstanceName,
			params.ruleName,
			titleCaseTypeName,
			writeResult.GetError()
		);

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

		WithMethod("getRule", this, ReadRuleValueApiProxy)
			.WithDescription(FormatString("Get rules for %s", titleCaseTypeName))
			.WithParameter("typeName", [&](LuaVariableInfo& vi) {
				vi.WithDescription(FormatString("The name of one of the %s as it appears in RULES.INI", titleCaseTypeName))
				  .WithType(LuaType::String);
			})
			.WithParameter("ruleName", [](LuaVariableInfo& vi) {
				vi.WithDescription("The rule name as it appears in RULES.INI")
				  .WithType(LuaType::String);
			})
			.WithReturnValue("ruleValue", [](LuaVariableInfo& vi) {
				vi.WithDescription("The current value for the specified rule")
				  .WithType(LuaType::Any);
			});
		

		WithMethod("setRule", this, WriteRuleValueApiProxy)
			.WithDescription(FormatString("Set rules for %s", titleCaseTypeName))
			.WithParameter("typeName", [&](LuaVariableInfo& vi) {
				vi.WithDescription(FormatString("The name of one of the %s as it appears in RULES.INI", titleCaseTypeName))
				  .WithType(LuaType::String);
			})
			.WithParameter("ruleName", [](LuaVariableInfo& vi) {
				vi.WithDescription("The rule name as it appears in RULES.INI")
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
