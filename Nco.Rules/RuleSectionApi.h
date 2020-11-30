#pragma once

#include <Logger.h>
#include <LuaApi.h>
#include <LuaObjectUtils.h>
#include <strings.h>

#include "IRulesIniSection.h"

class RuleSectionApi : public LuaApi
{
private:
    const int MAX_VALUE_LENGTH = 32;

    SectionName titleCaseSectionName;
    IRulesIniSection& section;

    static int ReadRuleLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RuleSectionApi, ReadRule);
    }

    static int WriteRuleLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RuleSectionApi, WriteRule);
    }

    static int GetRuleNamesLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RuleSectionApi, GetRuleNames);
    }

    RuleSectionApi(IRulesIniSection& section) : section(section), titleCaseSectionName(ToTitleCase(section.GetName()))
    {
        WithName(FormatString("%s rules", titleCaseSectionName));
        WithDescription(FormatString("Get and set %s rules", titleCaseSectionName));

        WithMethod(FormatString("get%sRule", titleCaseSectionName), this, ReadRuleLua)
            .WithDescription(FormatString("Get the current value of a %s rule", titleCaseSectionName))
            .WithParameter("ruleName", [](LuaVariableInfo& p) {
            p.WithDescription("The name as it appears in RULES.INI")
                .WithType(LuaType::String);
                });

        WithMethod(FormatString("set%sRule", titleCaseSectionName), this, WriteRuleLua)
            .WithDescription(FormatString("Set the current value of a %s rule", titleCaseSectionName))
            .WithParameter("ruleName", [](LuaVariableInfo& p) {
            p.WithDescription("The name as it appears in RULES.INI")
                .WithType(LuaType::String);
                })
            .WithParameter("ruleValue", [](LuaVariableInfo& p) {
                    p.WithDescription("A valid value for the specified rule")
                        .WithType(LuaType::Any);
                });

                WithMethod(FormatString("get%sRuleNames", titleCaseSectionName), this, GetRuleNamesLua)
                    .WithDescription(FormatString("Get a list of %s rules", titleCaseSectionName));
    }

    int ReadRule(ILuaStateWrapper& luaState)
    {
        LogTrace("get%sRule called from Lua", titleCaseSectionName);

        auto argCount = luaState.GetStackTop();

        if (argCount < 1)
        {
            luaState.RaiseError("get%sRule requires at least one argument", titleCaseSectionName);
            return 0;
        }

        auto& ruleNameResult = luaState.ReadString(1);
        auto ruleKey = section.BuildKey(ruleNameResult.GetValue());

        if (!section.HasRule(ruleKey))
        {
            luaState.RaiseError("Rule '%s' passed to get%sRule was not recognised", titleCaseSectionName, ruleNameResult.GetValue());

            delete &ruleNameResult;

            return 0;
        }

        auto& rule = section[ruleKey];

        if (rule.HasValue())
        {
            if (GetLogger().GetLogLevel() == TRACE)
            {
                auto valueStr = AllocateString(MAX_VALUE_LENGTH);

                rule.WriteValueToString(valueStr);

                LogTrace("get%sRule: Read rule value %s: %s", titleCaseSectionName, rule.GetStringKey(), valueStr);

                delete valueStr;
            }

            rule.PushValueOntoLuaState(luaState);
        }
        else if (rule.HasDefaultValue())
        {
            if (GetLogger().GetLogLevel() == TRACE)
            {
                auto valueStr = AllocateString(MAX_VALUE_LENGTH);

                rule.WriteDefaultValueToString(valueStr);

                LogTrace("get%sRule: Read rule value %s: %s", titleCaseSectionName, rule.GetStringKey(), valueStr);

                delete valueStr;
            }

            rule.PushDefaultValueOntoLuaState(luaState);
        }
        else
        {
            LogTrace("get%sRule: No value for rule %s, returning nil", titleCaseSectionName, rule.GetStringKey());

            luaState.WriteNil();
        }

        delete &ruleNameResult;

        return 1;
    }

    int WriteRule(ILuaStateWrapper& luaState)
    {
        LogTrace("set%sRule called from Lua", titleCaseSectionName);

        auto argCount = luaState.GetStackTop();

        if (argCount < 2)
        {
            luaState.RaiseError("set%sRule requires at least two arguments", titleCaseSectionName);
            return 0;
        }

        auto& ruleNameResult = luaState.ReadString(1);
        auto ruleKey = section.BuildKey(ruleNameResult.GetValue());

        if (!section.HasRule(ruleKey))
        {
            luaState.RaiseError("Rule '%s' passed to set%sRule was not recognised", ruleNameResult.GetValue(), titleCaseSectionName);

            delete &ruleNameResult;

            return 0;
        }

        auto& rule = section[ruleKey];

        auto setOk = rule.SetValueFromLuaState(luaState, 2);

        if (setOk && GetLogger().GetLogLevel() == TRACE)
        {
            auto valueStr = AllocateString(MAX_VALUE_LENGTH);

            rule.WriteValueToString(valueStr);

            LogTrace("set%sRule: Set rule value %s: %s", titleCaseSectionName, rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        delete &ruleNameResult;

        return 0;
    }

    int GetRuleNames(ILuaStateWrapper& lua)
    {
        lua.WriteArray(section.GetRuleNames());

        return 1;
    }

public:
    static ILuaApi& Build(IRulesIniSection& section)
    {
        return *(new RuleSectionApi(section));
    }

    ~RuleSectionApi()
    {
        if (titleCaseSectionName != NULL)
        {
            delete titleCaseSectionName;
        }
    }

};
