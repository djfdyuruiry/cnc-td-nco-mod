#pragma once

#include <Logger.h>
#include <LuaApi.h>
#include <LuaObjectUtils.h>
#include <strings.h>

#include "IRulesIniSection.h"
#include "IRulesRuntime.h"

class RulesApi : public LuaApi
{
private:
    const int MAX_VALUE_LENGTH = 32;

    IRulesRuntime& rulesRuntime;

    static int ReadRuleLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RulesApi, ReadRule);
    }

    static int WriteRuleLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RulesApi, WriteRule);
    }

    static int GetSectionRuleNamesLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RulesApi, GetSectionRuleNames);
    }

    static int GetSectionNamesLua(lua_State* lua)
    {
        return LUA_METHOD_PROXY(RulesApi, GetSectionNames);
    }

    RulesApi(IRulesRuntime& rulesRuntime) : rulesRuntime(rulesRuntime)
    {
        WithName("Rules");
        WithDescription("Get and set rule values, get rules section and entry info");

        WithMethod("getRule", this, ReadRuleLua)
            .WithDescription("Get the current value of a rule in a given section")
            .WithParameter("sectionName", [](auto& p) {
                p.WithDescription("The section name as it appears in RULES.INI")
                 .WithType(LuaType::String);
            })
            .WithParameter("ruleName", [](auto& p) {
                p.WithDescription("The section entry name as it appears in RULES.INI")
                 .WithType(LuaType::String);
            })
            .WithReturnValue("ruleValue", [](auto& p) {
                p.WithDescription("The current value for the specified rule")
                 .WithType(LuaType::Any);
            });

        WithMethod("setRule", this, WriteRuleLua)
            .WithDescription("Set the current value of a rule in a given section")
            .WithParameter("sectionName", [](auto& p) {
                p.WithDescription("The section name as it appears in RULES.INI")
                 .WithType(LuaType::String);
            })
            .WithParameter("ruleName", [](auto& p) {
                p.WithDescription("The section entry name as it appears in RULES.INI")
                 .WithType(LuaType::String);
            })
            .WithParameter("ruleValue", [](auto& p) {
                p.WithDescription("A valid value for the specified rule")
                 .WithType(LuaType::Any);
            });

        WithMethod("getSectionNames", this, GetSectionNamesLua)
            .WithDescription("Get a list of rules section names")
            .WithReturnValue("sectionNames", [](auto& p) {
                p.WithDescription("Table containing the section names as strings")
                 .WithType(LuaType::Table);
            });

        WithMethod("getRuleNames", this, GetSectionRuleNamesLua)
            .WithDescription("Get a list of entries in a given section")
            .WithParameter("sectionName", [](auto& p) {
                p.WithDescription("The section name as it appears in RULES.INI")
                 .WithType(LuaType::String);
            })
            .WithReturnValue("ruleNames", [](auto& p) {
                p.WithDescription("Table containing the entry names as strings")
                 .WithType(LuaType::Table);
            });
    }

    ResultWithValue<IRulesIniSection*>& ReadSectionParameter(ILuaStateWrapper& lua)
    {
        auto& sectionNameResult = lua.ReadString(1);

        auto sectionName = sectionNameResult.GetValue();

        delete& sectionNameResult;

        if (!rulesRuntime.GetRules().HasSection(sectionName))
        {
            lua.RaiseError("Rules section '%s' not found", sectionName);

            return ResultWithValue<IRulesIniSection*>::BuildWithError();
        }

        auto& section = rulesRuntime.GetRules()[sectionName];

        return ResultWithValue<IRulesIniSection*>::BuildWithValue(&section);
    }

    ResultWithValue<RulesIniRule*>& ReadSectionRuleParameter(ILuaStateWrapper& lua)
    {
        auto& sectionResult = ReadSectionParameter(lua);

        if (sectionResult.IsErrorResult())
        {
            delete &sectionResult;

            return ResultWithValue<RulesIniRule*>::BuildWithError();
        }

        auto& section = *(sectionResult.GetValue());

        delete &sectionResult;

        auto& ruleNameResult = lua.ReadString(2);

        auto ruleName = ruleNameResult.GetValue();
        auto ruleKey = section.BuildKey(ruleName);

        delete &ruleNameResult;

        if (!section.HasRule(ruleKey))
        {
            lua.RaiseError("Rule '%s' was not found in section '%s'", ruleName, section.GetName());

            return ResultWithValue<RulesIniRule*>::BuildWithError();
        }

        auto& rule = section[ruleKey];

        return ResultWithValue<RulesIniRule*>::BuildWithValue(&rule);
    }

    int ReadRule(ILuaStateWrapper& lua)
    {
        LogTrace("getRule called from Lua");

        auto &sectionRuleResult = ReadSectionRuleParameter(lua);

        if (sectionRuleResult.IsErrorResult())
        {
            delete &sectionRuleResult;

            return 0;
        }

        auto& rule = *(sectionRuleResult.GetValue());

        delete &sectionRuleResult;

        if (rule.HasValue())
        {
            if (GetLogger().GetLogLevel() == TRACE)
            {
                auto valueStr = AllocateString(MAX_VALUE_LENGTH);

                rule.WriteValueToString(valueStr);

                LogTrace("getRule: Read rule value %s: %s", rule.GetStringKey(), valueStr);

                delete valueStr;
            }

            rule.PushValueOntoLuaState(lua);
        }
        else if (rule.HasDefaultValue())
        {
            if (GetLogger().GetLogLevel() == TRACE)
            {
                auto valueStr = AllocateString(MAX_VALUE_LENGTH);

                rule.WriteDefaultValueToString(valueStr);

                LogTrace("getRule: Read rule value %s: %s", rule.GetStringKey(), valueStr);

                delete valueStr;
            }

            rule.PushDefaultValueOntoLuaState(lua);
        }
        else
        {
            LogTrace("getRule: No value for rule %s, returning nil", rule.GetStringKey());

            lua.WriteNil();
        }

        return 1;
    }

    int WriteRule(ILuaStateWrapper& lua)
    {
        LogTrace("setRule called from Lua");
    
        auto &sectionRuleResult = ReadSectionRuleParameter(lua);

        if (sectionRuleResult.IsErrorResult())
        {
            delete &sectionRuleResult;

            return 0;
        }

        auto &rule = *(sectionRuleResult.GetValue());

        delete &sectionRuleResult;

        auto setOk = rule.SetValueFromLuaState(lua, 3);

        if (setOk && GetLogger().GetLogLevel() == TRACE)
        {
            auto valueStr = AllocateString(MAX_VALUE_LENGTH);

            rule.WriteValueToString(valueStr);

            LogTrace("setRule: Set rule value %s: %s", rule.GetStringKey(), valueStr);

            delete valueStr;
        }

        return 0;
    }

    int GetSectionNames(ILuaStateWrapper& lua)
    {
        lua.WriteArray(rulesRuntime.GetRules().GetSectionNames());

        return 1;
    }

    int GetSectionRuleNames(ILuaStateWrapper& lua)
    {
        auto& sectionResult = ReadSectionParameter(lua);

        if (sectionResult.IsErrorResult())
        {
            delete &sectionResult;

            return 0;
        }

        auto& section = *(sectionResult.GetValue());

        delete &sectionResult;

        lua.WriteArray(section.GetRuleNames());

        return 1;
    }

public:
    static ILuaApi& Build(IRulesRuntime& rulesRuntime)
    {
        return *(new RulesApi(rulesRuntime));
    }

};
