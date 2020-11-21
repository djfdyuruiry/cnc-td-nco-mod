#pragma once

#include <ILuaRuntime.h>
#include <LuaApi.h>
#include <LuaObjectUtils.h>

#include "lua_events.h"

class InfoApi : public LuaApi
{
private:
    static int GetEventNamesProxy(lua_State* lua)
    {
        return LUA_METHOD_PROXY(InfoApi, GetEventNames);
    }

    ILuaRuntime& runtime;

    InfoApi(ILuaRuntime& runtime) : runtime(runtime)
    {
        WithName("Info");
        WithDescription("General info functions");

        WithMethod("getEventNames", this, GetEventNamesProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Get a list of game events");
            // TODO: return type
        });
    }

    int GetEventNames(ILuaStateWrapper& lua)
    {
        auto& events = std::vector<const char*>();
        auto eventNames = Get_Event_Names();

        for (auto i = 0; i < Get_Event_Count(); i++)
        {
            events.push_back(eventNames[i]);
        }

        lua.WriteArray(events);

        return 1;
    }

public:
    static ILuaApi& Build(ILuaRuntime& runtime)
    {
        return *(new InfoApi(runtime));
    }

};
