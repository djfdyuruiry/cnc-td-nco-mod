#pragma once

#include <LuaApi.h>
#include <LuaObjectUtils.h>

class InfoApi : public LuaApi
{
private:
    static int GetEventNamesProxy(lua_State* lua)
    {
        return LUA_METHOD_PROXY(InfoApi, GetEventNames);
    }

    InfoApi()
    {
        WithName("Info");
        WithDescription("General info functions");

        WithMethod("getEventNames", this, GetEventNamesProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Get a list of game events");
        });
    }

    int GetEventNames(ILuaStateWrapper& lua)
    {
        auto& events = std::vector<const char*>();

        for (auto i = 0; i < LUA_EVENT_COUNT; i++)
        {
            events.push_back(LUA_EVENTS[i]);
        }

        lua.WriteArray(events);

        return 1;
    }

public:
    static ILuaApi& Build()
    {
        return *(new InfoApi());
    }

};
