#pragma once

#include <Logger.h>
#include <Optional.h>
#include <Strings.h>

#include "LuaApi.h"
#include "LuaStateWrapper.h"
#include "TypeApiParameters.h"

template<class T> class TypeApi : public LuaApi
{
private:
	const char* typeName;
	const char* titleCaseTypeName;

	TypeApiParameters<T>& GetParameters(const char* operation, ILuaStateWrapper& luaState)
	{
		Log_Trace("%s%sRule called from Lua", operation, titleCaseTypeName, operation);

		int argCount = luaState.GetStackTop();

		if (argCount < 2)
		{
			luaState.RaiseError("%s%sRule requires exactly two arguments", operation, titleCaseTypeName);

			return TypeApiParameters::BuildInvalid();
		}

		auto& typeInstanceNameResult = luaState.ReadString(1);

		if (typeInstanceNameResult.IsErrorResult() || String_Is_Empty(typeInstanceNameResult.GetValue()))
		{
			luaState.RaiseError("%s%sRule argument `%s` was nil or blank", operation, titleCaseTypeName, paramName);

			delete &typeInstanceNameResult;

			return TypeApiParameters::BuildInvalid();
		}

		auto& typeInstance = ParseType(typeInstanceNameResult.GetValue());

		if (typeInstance == NULL)
		{
			delete &typeInstanceNameResult;

			return TypeApiParameters::BuildInvalid();
		}

		auto& ruleNameParameterResult = luaState.ReadString(2);

		if (ruleNameParameterResult.IsErrorResult() || String_Is_Empty(ruleNameParameterResult.GetValue()))
		{
			luaState.RaiseError("%s%sRule argument `ruleName` was nil", operation, titleCaseTypeName);

			delete &typeInstanceNameResult;
			delete &ruleNameParameterResult;

			return TypeApiParameters::BuildInvalid();
		}

		if (!validateRule(ruleNameParameterResult.GetValue()))
		{
			luaState.RaiseError(
				"rule name type passed %s%sRule was not recognised for this type: %s",
				operation,
				titleCaseTypeName,
				ruleNameParameterResult.GetValue()
			);

			delete &typeInstanceNameResult;
			delete &ruleNameParameterResult;

			return TypeApiParameters::BuildInvalid();
		}

		auto& params TypeApiParameters::BuildValid(
			typeInstanceNameResult.GetValue(),
			ruleNameParameterResult.GetValue(),
			typeInstance
		);

		delete &typeInstanceNameResult;
		delete &ruleNameParameterResult;

		return params;
	}

	int ReadRule(lua_State* lua)
	{
		auto& luaState = LuaStateWrapper::Build(lua, false);
		auto& params = GetParameters("get", luaState);

		if (!params.IsValid())
		{
			delete &luaState;
			delete &params;

			return 0;
		}

        Log_Debug("get%sRule => attempting to read value of rule '%s'", titleCaseTypeName, ruleNameParameterResult.GetValue());

        auto ruleMatched = ReadRule(params.typeInstance, params.ruleName);

        if (ruleMatched)
        {
            Log_Debug("get%sRule => Rule matched in %s Type", titleCaseTypeName, typeName);
			
			delete &luaState;
			delete &params;

            return 1;
        }

        luaState.RaiseError("rule name type passed get%sRule was not matched: %s", titleCaseTypeName, ruleNameParameterResult.GetValue());

		delete &luaState;
		delete &params;

		return 0;
	}

	int WriteRule(lua_State* lua)
	{
		auto& luaState = LuaStateWrapper::Build(lua, false);
		auto& params = GetParameters("set", luaState);

		if (!params.IsValid())
		{
			delete &luaState;
			delete &params;

			return 0;
		}

        Log_Debug("set%sRule => attempting to write value of rule '%s'", titleCaseTypeName, ruleNameParameterResult.GetValue());

		auto& ruleValueResult = luaState.ReadString(3);

		if (ruleValueResult.IsErrorResult() || String_Is_Empty(ruleValueResult.GetValue()))
		{
			luaState.RaiseError("set%sRule argument `ruleValue` was nil or blank", typeName);
			
			delete &luaState;
			delete &params;
			delete &ruleValueResult;

			return 0;
		}

        auto ruleMatched = WriteRule(params.typeInstance, params.ruleName, ruleValueResult.GetValue());

        if (ruleMatched)
        {
            Log_Debug("set%sRule => Rule matched in %s Type", titleCaseTypeName, typeName);
			
			delete &luaState;
			delete &params;
			delete &ruleValueResult;

            return 1;
        }

        luaState.RaiseError("rule name type passed set%sRule was not matched: %s", titleCaseTypeName, ruleNameParameterResult.GetValue());

		delete &luaState;
		delete &params;
		delete &ruleValueResult;

		return 0;
	}

protected:
	TypeApi(const char* typeName) : typeName(typeName), titleCaseTypeName(ToTitleCase(typeName))
	{
		WithName(FormatString("%s rules", titleCaseTypeName));
		WithDescription(FormatString("%s rule info and control functions", titleCaseTypeName));

		WithFunction(FormatString("get%sNameRule", titleCaseTypeName), &ReadRule, [](LuaFunctionInfo & i) -> {
			i.WithDescription(FormatString("Set a rule for a given %s", titleCaseTypeName))
			 .WithParameter("ruleName", [](LuaVariableInfo& vi) -> {
			     vi.WithDescription("The name as it appears in RULES.INI")
				   .WithType(LuaType::String)
			 })
			 .WithReturnValue("ruleValue", [](LuaVariableInfo& vi) -> {
			     vi.WithDescription("The current value for the specified rule")
				   .WithType(LuaType::Any)
			 });
		});

		WithFunction(FormatString("set%sNameRule", titleCaseTypeName), &WriteRule, [](LuaFunctionInfo & i) -> {
			i.WithDescription(FormatString("Get a rule for a given %s", titleCaseTypeName))
			  .WithParameter("ruleName", [](LuaVariableInfo& vi) -> {
			      vi.WithDescription("The name as it appears in RULES.INI")
				    .WithType(LuaType::String)
			  })
			  .WithParameter("ruleValue", [](LuaVariableInfo& vi) -> {
			      vi.WithDescription("A valid value for the specified rule")
				    .WithType(LuaType::Any)
			  });
		});

		delete titleCaseTypeName;
	}

	virtual bool validateRule(const char* ruleName) = 0;

	virtual T& ParseType(const char* typeName) = 0;

	virtual bool ReadRule(T& typeInstance, const char* ruleName) = 0;

	virtual bool WriteRule(T& typeInstance, const char* ruleName, const char* ruleValue) = 0;

};
