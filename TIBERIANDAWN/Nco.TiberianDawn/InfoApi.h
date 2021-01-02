#pragma once

#include <ILuaRuntime.h>
#include <LuaApi.h>
#include <LuaObjectUtils.h>

#include "lua_events.h"

class InfoApi : public LuaApi
{
private:
    static int GetTypeNamesProxy(lua_State* lua)
    {
        return LUA_METHOD_PROXY(InfoApi, GetTypeNamesLua);
    }

    static int GetEventNamesProxy(lua_State* lua)
    {
        return LUA_METHOD_PROXY(InfoApi, GetEventNamesLua);
    }

    ILuaRuntime& runtime;

    InfoApi(ILuaRuntime& runtime) : runtime(runtime)
    {
        WithName("Info");
        WithDescription("General info functions");

        WithMethod("getEventNames", this, GetEventNamesProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Get a list of game events")
             .WithReturnValue("eventNames", [](LuaVariableInfo& v) {
                v.WithDescription("Table of strings containing the names of game events")
                 .WithType(LuaType::Table);
             });
        });

        WithMethod("getTypeNames", this, GetTypeNamesProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Get a list of types in the game (units, infantry etc.)")
             .WithReturnValue("eventNames", [](LuaVariableInfo& v) {
                v.WithDescription("Table of strings containing the names of game types")
                 .WithType(LuaType::Table);
             });
        });
    }

    int GetEventNamesLua(ILuaStateWrapper& lua)
    {
        auto& events = std::vector<const char*>();
        auto eventNames = GetEventNames();

        for (auto i = 0; i < GetEventCount(); i++)
        {
            events.push_back(eventNames[i]);
        }

        lua.WriteArray(events);

        return 1;
    }

    int GetTypeNamesLua(ILuaStateWrapper& lua)
    {
        std::vector<const char*> names =
        {
            "Warheads",
            "Bullets",
            "Weapons",
            "Buildings",
            "Infantry",
            "Units",
            "Aircraft"
        };

        lua.WriteArray(names);

        return 1;
    }

public:
    static ILuaApi& Build(ILuaRuntime& runtime)
    {
        return *(new InfoApi(runtime));
    }

};
