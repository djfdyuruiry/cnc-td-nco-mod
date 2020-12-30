#pragma once

#include <vector>

#include <ILuaStateWrapper.h>
#include <Logger.h>
#include <LuaApi.h>
#include <LuaFunctionInfo.h>
#include <LuaVariableInfo.h>
#include <strings.h>

#include "../FUNCTION.H"

#include "game_utils.h"
#include "rules_ini_superweapon.h"
#include "TiberianDawnNcoRuntime.h"

#define GAME_API_METHOD(f, fl) static int f(lua_State* lua) \
    { \
        return LUA_METHOD_PROXY(GameApi, fl); \
    }

class GameApi : public LuaApi
{
private:
    GAME_API_METHOD(ShowGameMessageProxy, ShowGameMessageLua)

    static int HideMapLua(lua_State* _)
    {
        LogTrace("HideMapLua called from Lua");

        PushHideMapUiMessage();

        return 0;
    }

    static int RevealMapLua(lua_State* _)
    {
        LogTrace("RevealMapLua called from Lua");

        PushRevealMapUiMessage();

        return 0;
    }

    static int RefreshMapLua(lua_State* _)
    {
        LogTrace("Lua_Refresh_Map called from Lua");

        PushRefreshMapUiMessage();

        return 0;
    }

    static int RefreshSidebarLua(lua_State* _)
    {
        LogTrace("Lua_Refresh_Sidebar called from Lua");

        PushRefreshSidebarUiMessage();

        return 0;
    }

    GAME_API_METHOD(GetPlayerHouse, GetPlayerBaseHouseLua)

    GAME_API_METHOD(GetPlayerBaseHouse, GetPlayerBaseHouseLua)

    GAME_API_METHOD(GetActiveHouses, GetActiveHousesLua)

    GAME_API_METHOD(ModifiyHouseCredits, ModifiyHouseCreditsLua)

    GAME_API_METHOD(EnableHouseSuperWeapon, EnableHouseSuperWeaponLua)

    GAME_API_METHOD(ChargeHouseSuperWeapon, ChargeHouseSuperWeaponLua)

    GAME_API_METHOD(DisableHouseSuperWeapon, DisableHouseSuperWeaponLua)

    static int ClearGameLoopMessagesLua(lua_State* _)
    {
        ClearGameLoopMessages();

        return 0;
    }

    static int ClearGameUiMessagesLua(lua_State* _)
    {
        ClearGameUiMessages();

        return 0;
    }

    GAME_API_METHOD(ClearHouseMessagesProxy, ClearHouseMessagesLua)

    ResultWithValue<HousesType>& ParseHouseTypeLua(
        ILuaStateWrapper& lua,
        int index,
        const char* callingFunctionName
    )
    {
        auto& houseTypeStringResult = lua.ReadString(index);
        auto houseTypeString = houseTypeStringResult.GetValue();

        delete& houseTypeStringResult;

        if (StringIsEmpty(houseTypeString))
        {
            return ResultWithValue<HousesType>::BuildWithError(
                "%s parameter `houseName` was blank",
                callingFunctionName
            );
        }

        auto& houseTypeResult = NcoTypeConverter().Parse<HousesType>(houseTypeString);

        if (houseTypeResult.IsErrorResult())
        {
            delete& houseTypeResult;

            return ResultWithValue<HousesType>::BuildWithError(
                "%s parameter `houseName` was not a valid house: %s",
                callingFunctionName,
                houseTypeString
            );
        }

        return houseTypeResult;
    }

    int ShowGameMessageLua(ILuaStateWrapper& lua)
    {
        LogTrace("Lua_Show_Game_Message called from Lua");

        auto& messageResult = lua.ReadString(1);

        auto message = messageResult.GetValue();
        auto& durationInSecondsResult = lua.ReadDouble(2);

        delete& messageResult;

        if (durationInSecondsResult.GetValue() < 0.1)
        {
            delete& durationInSecondsResult;

            lua.RaiseError("showGameMessage parameter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");

            return 0;
        }

        PushShowNotificationUiMessage(message, durationInSecondsResult.GetValue());

        delete& durationInSecondsResult;

        return 0;
    }

    int GetActiveHousesLua(ILuaStateWrapper& lua)
    {
        LogTrace("Lua_Get_Active_Houses called from Lua");

        auto activeHousesCount = Houses.Count();
        auto houses = std::vector<const char*>();

        for (auto i = 0; i < activeHousesCount; i++)
        {
            auto house = Houses.Raw_Ptr(i);

            houses.push_back(
                NcoTypeConverter().ToStringOrDefault(house->Class->House)
            );
        }

        lua.WriteArray(houses);

        return 1;
    }

    int GetPlayerBaseHouseLua(ILuaStateWrapper& lua)
    {
        LogTrace("GetPlayerBaseHouseLua called from Lua");

        auto playerBaseHouse = NcoTypeConverter().ToStringOrDefault(PlayerPtr->ActLike);

        lua.WriteString(playerBaseHouse);

        return 1;
    }

    int GetPlayerHouseLua(ILuaStateWrapper& lua)
    {
        LogTrace("GetPlayerHouseLua called from Lua");

        auto playerHouse = NcoTypeConverter().ToStringOrDefault(PlayerPtr->Class->House);

        lua.WriteString(playerHouse);

        return 1;
    }

    int ModifiyHouseCreditsLua(ILuaStateWrapper& lua)
    {
        LogTrace("Lua_Modify_House_Credits called from Lua");

        auto& houseTypeResult = ParseHouseTypeLua(lua, 1, "modifyHouseCredits");

        if (houseTypeResult.IsErrorResult())
        {
            lua.RaiseError(houseTypeResult);

            delete& houseTypeResult;

            return 0;
        }

        auto houseType = houseTypeResult.GetValue();

        delete& houseTypeResult;

        auto& creditsModifierResult = lua.ReadInteger(2);

        auto house = GetHouseByType(houseType);
        auto creditsModifier = creditsModifierResult.GetValue();

        delete& creditsModifierResult;

        if (house == NULL)
        {
            lua.RaiseError("House '%s' is not in the current game", NcoTypeConverter().ToStringOrDefault(houseType));

            return 0;
        }

        house->Credits += creditsModifier;

        lua.WriteInteger(house->Credits);

        return 1;
    }

    HouseClass* GetHouseByType(HousesType houseType)
    {
        for (auto i = 0; i < Houses.Count(); i++)
        {
            auto house = Houses.Raw_Ptr(i);

            if (house != NULL && house->Class->House == houseType)
            {
                return house;
            }
        }

        return NULL;
    }

    ResultWithValue<SuperweaponType>& ParseSuperweaponTypeLua(ILuaStateWrapper& lua, int index, const char* callingFunctionName)
    {
        auto& superWeaponNameResult = lua.ReadString(index);
        auto name = superWeaponNameResult.GetValue();

        delete &superWeaponNameResult;

        if (StringIsEmpty(name))
        {
            return ResultWithValue<SuperweaponType>::BuildWithError(
                "%s parameter `superWeaponName` was empty",
                callingFunctionName
            );
        }

        auto weapon = NO_SUPERWEAPON;
        auto uppercaseName = ConvertStringToUpperCase(name);

        if (StringsAreEqual(uppercaseName, AIRSTRIKE_SECTION_NAME_UPPER))
        {
            weapon = AIRSTRIKE;
        }
        else if (StringsAreEqual(uppercaseName, ION_CANNON_SECTION_NAME_UPPER))
        {
            weapon = ION_CANNON;
        }
        else if (StringsAreEqual(uppercaseName, NUCLEAR_STRIKE_SECTION_NAME_UPPER))
        {
            weapon = NUCLEAR_STRIKE;
        }
        else
        {
            delete uppercaseName;

            return ResultWithValue<SuperweaponType>::BuildWithError(
                "%s parameter `superWeaponName` was not recognised as a superweapon name: %s",
                callingFunctionName,
                name
            );
        }

        delete uppercaseName;

        return ResultWithValue<SuperweaponType>::BuildWithValue(weapon);
    }

    int SendHouseSuperWeaponMessageLua(
        ILuaStateWrapper& lua,
        const char* callingFunctionName,
        const char* callingLuaFunctionName,
        SuperweaponMessageType messageType
    ) {
        LogTrace("%s called from Lua", callingFunctionName);

        auto& houseResult = ParseHouseTypeLua(lua, 1, callingLuaFunctionName);

        if (houseResult.IsErrorResult())
        {
            lua.RaiseError(houseResult);

            delete &houseResult;

            return 0;
        }

        auto house = houseResult.GetValue();

        delete &houseResult;

        auto& superweaponResult = ParseSuperweaponTypeLua(lua, 2, callingLuaFunctionName);

        if (superweaponResult.IsErrorResult())
        {
            lua.RaiseError(superweaponResult.GetError());

            delete &superweaponResult;

            return 0;
        }

        auto superweapon = superweaponResult.GetValue();

        delete &superweaponResult;

        auto oneTime = false;

        if (lua.GetStackTop() > 2)
        {
            oneTime = lua.ReadBool(3).GetValue();
        }

        auto& message = HouseMessage{};

        message.oneTime = oneTime;
        message.superweapon = superweapon;
        message.superweaponMessage = messageType;

        PushHouseMessage(house, SUPERWEAPON_MESSAGE, message);

        return 0;
    }

    int EnableHouseSuperWeaponLua(ILuaStateWrapper& lua) {
        return SendHouseSuperWeaponMessageLua(
            lua,
            "EnableHouseSuperWeaponLua",
            "enableSuperWeaponForHouse",
            ENABLE_SUPERWEAPON
        );
    }

    int ChargeHouseSuperWeaponLua(ILuaStateWrapper& lua) {
        return SendHouseSuperWeaponMessageLua(
            lua,
            "ChargeHouseSuperWeaponLua",
            "chargeSuperWeaponForHouse",
            CHARGE_SUPERWEAPON
        );
    }

    int DisableHouseSuperWeaponLua(ILuaStateWrapper& lua) {
        return SendHouseSuperWeaponMessageLua(
            lua,
            "DisableHouseSuperWeaponLua",
            "disableSuperWeaponForHouse",
            DISABLE_SUPERWEAPON
        );
    }

    int ClearHouseMessagesLua(ILuaStateWrapper& lua)
    {
        LogTrace("Lua_Clear_House_Messages called from Lua");

        auto& houseResult = ParseHouseTypeLua(lua, 1, "clearHouseMessages");

        if (houseResult.IsErrorResult())
        {
            lua.RaiseError(houseResult.GetError());

            delete& houseResult;

            return 0;
        }

        ClearHouseMessages(houseResult.GetValue());

        delete& houseResult;

        return 0;
    }


    GameApi()
    {
        WithName("Game");
        WithDescription("Game info and control functions");

        WithMethod("showGameMessage", this, ShowGameMessageProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Show the player an in-game message")
             .WithParameter("message", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             })
             .WithParameter("durationInSeconds", [](LuaVariableInfo& p) {
                p.WithDescription("Display the message for this many seconds; minimum value is `0.1`. Note: this function is async")
                 .WithType(LuaType::Number);
             });
        });

        WithFunction("refreshSidebar", RefreshSidebarLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Force a refresh of the sidebar; useful if you make a rule change that affects what the user can build and want to reflect that right away. Note: this function is async");
        });

        WithFunction("revealEntireMap", RevealMapLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Reveal the entire map to the player; this is permanent unless you call `hideEntireMap`. Note: this function is async");
        });

        WithFunction("hideEntireMap", HideMapLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Hide any areas of the map without player units/buildings from the player. Note: this function is async");
        });

        WithMethod("getPlayerHouse", this, GetPlayerHouse, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the current player house")
             .WithReturnValue("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("The house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithMethod("getPlayerBaseHouse", this, GetPlayerBaseHouse, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the base house for the current player house. Every house has a base house which it acts like, usually NOD or GDI.")
             .WithReturnValue("baseHouseName", [](LuaVariableInfo& p) {
                p.WithDescription("The base house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithMethod("getActiveHouses", this, GetActiveHouses, [](LuaFunctionInfo& f) {
            f.WithDescription("Get all houses that are active in the current scenario")
             .WithReturnValue("activeHouseNames", [](LuaVariableInfo& p) {
                p.WithDescription("List of strings, containing the names of active houses.")
                 .WithType(LuaType::Table);
             });
        });

        WithMethod("modifyHouseCredits", this, ModifiyHouseCredits, [](LuaFunctionInfo& f) {
            f.WithDescription("Write a info line to the log file")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to modify")
                 .WithType(LuaType::String);
             })
             .WithParameter("amount", [](LuaVariableInfo& p) {
                p.WithDescription("Amount of credits to either add or subtract for current house balance; use negative numbers to deduct credits.")
                 .WithType(LuaType::Number);
             });
        });

        WithMethod("enableSuperweaponForHouse", this, EnableHouseSuperWeapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Enable a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to enable superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithMethod("chargeSuperweaponForHouse", this, ChargeHouseSuperWeapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Charge a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to charge superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithMethod("disableSuperweaponForHouse", this, DisableHouseSuperWeapon, [](LuaFunctionInfo& f) {
            f.WithDescription("Disable a superweapon for a given house. Note: this function is async")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the house to disable superweapon for")
                 .WithType(LuaType::String);
             })
             .WithParameter("superweaponName", [](LuaVariableInfo& p) {
                p.WithDescription("Name of the superweapon as it appears in the RULES.INI file")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("clearGameUiMessages", ClearGameUiMessagesLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending async game UI changes (refresh sidebar, reveal map etc.)");
        });

        WithFunction("clearGameLoopMessages", ClearGameLoopMessagesLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending async game events (on game tick etc.)");
        });

        WithMethod("clearHouseMessages", this, ClearHouseMessagesProxy, [](LuaFunctionInfo& f) {
            f.WithDescription("Cancel any pending events for a given house (enable/disable superweapon etc.)")
             .WithParameter("houseName", [](LuaVariableInfo& p) {
                 p.WithDescription("Name of the house to cancelevents for")
                  .WithType(LuaType::String);
             });
        });
    }

public:
    static ILuaApi& Build()
    {
        return *(new GameApi());
    }

};
