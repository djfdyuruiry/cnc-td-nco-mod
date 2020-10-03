#pragma once

#include <Strings.h>

#include "ILuaApi.h"
#include "LuaStateWrapper.h"

template<class T> class TypeApi : public ILuaApi
{
private:
	const char* typeName;

	int ReadRule(lua_State* lua)
	{
		auto& luaWrapper = LuaStateWrapper::Build(lua);

		delete* luaWrapper;

		return 0;
	}

	int WriteRule(lua_State* lua)
	{
		auto& luaWrapper = LuaStateWrapper::Build(lua);

		delete *luaWrapper;

		return 0;
	}

protected:
	TypeApi(const char* typeName) : typeName(typeName)
	{
		auto titleCaseName = ToTitleCase(typeName);

		WithFunction(FormatString("get%sNameRule", titleCaseName), &ReadRule, [](LuaFunctionInfo & i) -> {
			i.WithDescription(FormatString("Set a rule for a given %s", titleCaseName))
			  .WithParameter("ruleName", [](LuaVariableInfo& vi) -> {
				vi.WithDescription("The name as it appears in RULES.INI")
				  .WithType(LuaType::String)
			  })
			  .WithReturnValue("ruleValue", [](LuaVariableInfo& vi) -> {
			    vi.WithDescription("The current value for the specified rule")
				  .WithType(LuaType::Any)
			  });
		});

		WithFunction(FormatString("set%sNameRule", titleCaseName), &WriteRule, [](LuaFunctionInfo & i) -> {
			i.WithDescription(FormatString("Get a rule for a given %s", titleCaseName))
			  .WithParameter("ruleName", [](LuaVariableInfo& vi) -> {
				vi.WithDescription("The name as it appears in RULES.INI")
				  .WithType(LuaType::String)
			  })
			  .WithParameter("ruleValue", [](LuaVariableInfo& vi) -> {
			    vi.WithDescription("A valid value for the specified rule")
				  .WithType(LuaType::Any)
			  });
		});
	}

	virtual void ReadRule(T typeInstance, const char* ruleName) = 0;

	virtual void WriteRule(T typeInstance, const char* ruleName, const char* ruleValue) = 0;

public:
	static TypeApi& Build(const char* typeName)
	{
		return *(new TypeApi(typeName));
	}

};
