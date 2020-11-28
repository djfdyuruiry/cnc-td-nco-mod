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
#include "parse.h"
#include "rules_ini_superweapon.h"
#include "TiberianDawnNcoRuntime.h"

class GameApi : public LuaApi
{
private:
    static LuaResultWithValue<HousesType>& ParseHouseTypeLua(ILuaStateWrapper& lua, int index, const char* callingFunctionName)
    {
        auto& houseTypeStringResult = lua.ReadString(index);

        if (houseTypeStringResult.IsErrorResult())
        {
            auto& result = LuaResultWithValue<HousesType>::BuildWithError(houseTypeStringResult.GetError());

            delete &houseTypeStringResult;

            return result;
        }

        auto houseTypeString = houseTypeStringResult.GetValue();
        
        delete &houseTypeStringResult;

        bool parseError = false;
        auto upperHouseTypeString = ConvertStringToUpperCase(houseTypeString);
        auto houseType = ParseHouseType(upperHouseTypeString, &parseError);

        delete upperHouseTypeString;

        if (parseError)
        {
            return LuaResultWithValue<HousesType>::BuildWithError(
                FormatString(
                    "%s parameter `houseName` was not a valid house: %s",
                    callingFunctionName,
                    houseTypeString
                )
            );
        }

        return LuaResultWithValue<HousesType>::BuildWithValue(houseType);
    }

    static int ClearHouseMessagesLua(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("Lua_Clear_House_Messages called from Lua");

        auto argCount = lua.GetStackTop();

        if (argCount < 1)
        {
            lua.RaiseError("clearHouseMessages requires at least one argument");

            return 0;
        }

        bool parseError = false;
        auto& houseResult = ParseHouseTypeLua(lua, 1, "clearHouseMessages");

        if (parseError)
        {
            lua.RaiseError(houseResult.GetError());

            delete &houseResult;

            return 0;
        }

        ClearHouseMessages(houseResult.GetValue());

        return 0;
    }

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


    static HouseClass* GetHouseByType(HousesType houseType, bool* housePresent)
    {
        for (auto i = 0; i < Houses.Count(); i++)
        {
            auto house = Houses.Raw_Ptr(i);

            if (house != NULL && house->Class->House == houseType)
            {
                if (housePresent != NULL)
                {
                    *housePresent = true;
                }

                return house;
            }
        }

        if (housePresent != NULL)
        {
            *housePresent = false;
        }

        return NULL;
    }

    static SuperweaponType ParseSuperweaponTypeLua(int index, const char* callingFunctionName, bool* invalidValue)
    {
        auto& lua = NcoLuaRuntime().GetState();
        auto& superWeaponNameResult = lua.ReadString(index);

        if (superWeaponNameResult.IsErrorResult() || StringIsEmpty(superWeaponNameResult.GetValue()))
        {
            lua.RaiseError("%s parameter `superWeaponName` was nil or empty", callingFunctionName);

            if (invalidValue != NULL)
            {
                *invalidValue = true;
            }

            delete &superWeaponNameResult;

            return NO_SUPERWEAPON;
        }

        auto weapon = NO_SUPERWEAPON;
        auto uppercaseName = ConvertStringToUpperCase(superWeaponNameResult.GetValue());

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
            lua.RaiseError(
                "%s parameter `superWeaponName` was not recognised as a superweapon name: %s",
                callingFunctionName,
                superWeaponNameResult.GetValue()
            );
        }

        delete uppercaseName;
        delete &superWeaponNameResult;

        return weapon;
    }

    static int SendHouseSuperWeaponMessageLua(
        const char* callingFunctionName,
        const char* callingLuaFunctionName,
        SuperweaponMessageType messageType
    ) {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("%s called from Lua", callingFunctionName);

        auto argCount = lua.GetStackTop();

        if (argCount < 2)
        {
            lua.RaiseError("%s requires at least two arguments", callingLuaFunctionName);
            return 0;
        }

        bool parseError = false;
        auto& houseResult = ParseHouseTypeLua(lua, 1, callingLuaFunctionName);

        if (houseResult.IsErrorResult())
        {
            lua.RaiseError(houseResult);

            delete &houseResult;

            return 0;
        }

        auto house = houseResult.GetValue();
        auto superweapon = ParseSuperweaponTypeLua(2, callingLuaFunctionName, &parseError);
        
        delete &houseResult;

        if (parseError)
        {
            return 0;
        }

        auto oneTime = false;

        if (argCount > 2)
        {
            if (!lua.IsBool(3))
            {
                lua.RaiseError("%s parameter `oneTimeOnly` must be a boolean", callingLuaFunctionName);
                return 0;
            }

            oneTime = lua.ReadBool(3).GetValue();
        }

        HouseMessage message{};

        message.oneTime = oneTime;
        message.superweapon = superweapon;
        message.superweaponMessage = messageType;

        PushHouseMessage(house, SUPERWEAPON_MESSAGE, message);

        return 0;
    }

    static int DisableHouseSuperWeaponLua(lua_State* _) {
        return SendHouseSuperWeaponMessageLua(
            "Lua_Enable_House_Super_Weapon_Message",
            "disableSuperWeaponForHouse",
            DISABLE_SUPERWEAPON
        );
    }

    static int ChargeHouseSuperWeaponLua(lua_State* _) {
        return SendHouseSuperWeaponMessageLua(
            "Lua_Enable_House_Super_Weapon_Message",
            "chargeSuperWeaponForHouse",
            CHARGE_SUPERWEAPON
        );
    }

    static int EnableHouseSuperWeaponLua(lua_State* _) {
        return SendHouseSuperWeaponMessageLua(
            "Lua_Enable_House_Super_Weapon_Message",
            "enableSuperWeaponForHouse",
            ENABLE_SUPERWEAPON
        );
    }

    static int ModifiyHouseCreditsLua(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("Lua_Modify_House_Credits called from Lua");

        int argCount = lua.GetStackTop();

        if (argCount < 2)
        {
            lua.RaiseError("modifyHouseCredits requires exactly two arguments");
            return 0;
        }

        auto& houseTypeResult = ParseHouseTypeLua(lua, 1, "modifyHouseCredits");

        if (houseTypeResult.IsErrorResult())
        {
            lua.RaiseError(houseTypeResult);

            delete &houseTypeResult;

            return 0;
        }

        auto houseType = houseTypeResult.GetValue();

        delete &houseTypeResult;

        if (!lua.IsNumber(2))
        {
            lua.RaiseError("modifyHouseCredits parameter `creditsModifier` must be a number");
            return 0;
        }

        auto& creditsModifierResult = lua.ReadInteger(2);

        bool housePresent = false;
        auto house = GetHouseByType(houseType, &housePresent);
        auto creditsModifier = creditsModifierResult.GetValue();

        delete &creditsModifierResult;

        if (!housePresent)
        {
            LogWarn("Ignoring modifyHouseCredits call as house '%s' is not in the current game", HouseTypeToString(houseType));

            return 0;
        }

        house->Credits += creditsModifier;

        lua.WriteInteger(house->Credits);

        return 1;
    }

    static int Lua_Get_Active_Houses(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("Lua_Get_Active_Houses called from Lua");

        auto activeHousesCount = Houses.Count();
        auto houses = std::vector<const char*>();

        for (auto i = 0; i < activeHousesCount; i++)
        {
            auto house = Houses.Raw_Ptr(i);

            houses.push_back(
                HouseTypeToString(house->Class->House)
            );
        }

        lua.WriteArray(houses);

        return 1;
    }

    static int GetPlayerBaseHouseLua(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("GetPlayerBaseHouseLua called from Lua");

        auto playerBaseHouse = HouseTypeToString(PlayerPtr->ActLike);

        lua.WriteString(playerBaseHouse);

        return 1;
    }

    static int GetPlayerHouseLua(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("GetPlayerHouseLua called from Lua");

        auto playerHouse = HouseTypeToString(PlayerPtr->Class->House);

        lua.WriteString(playerHouse);

        return 1;
    }

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

    static int ShowGameMessageLua(lua_State* _)
    {
        auto& lua = NcoLuaRuntime().GetState();
        LogTrace("Lua_Show_Game_Message called from Lua");

        int argCount = lua.GetStackTop();

        if (argCount < 2)
        {
            lua.RaiseError("showGameMessage requires exactly two arguments");
            return 0;
        }

        if (!lua.IsString(1))
        {
            lua.RaiseError("showGameMessage parameter `message` must be a string");
            return 0;
        }

        if (!lua.IsNumber(2))
        {
            lua.RaiseError("showGameMessage parameter `durationInSeconds` must be a number");
            return 0;
        }

        auto& messageResult = lua.ReadString(1);

        if (messageResult.IsErrorResult() || messageResult.GetValue() == NULL)
        {
            delete& messageResult;

            lua.RaiseError("showGameMessage parameter `message` was nil");
            return 0;
        }

        auto message = messageResult.GetValue();
        auto& durationInSecondsResult = lua.ReadDouble(2);
        
        delete &messageResult;

        if (durationInSecondsResult.IsErrorResult() || durationInSecondsResult.GetValue() < 0.1)
        {
            delete &durationInSecondsResult;

            lua.RaiseError("showGameMessage parameter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");
            return 0;
        }

        PushShowNotificationUiMessage(message, durationInSecondsResult.GetValue());

        delete &durationInSecondsResult;

        return 0;
    }

    GameApi()
    {
        WithName("Game");
        WithDescription("Game info and control functions");

        WithFunction("showGameMessage", ShowGameMessageLua, [](LuaFunctionInfo& f) {
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

        WithFunction("getPlayerHouse", GetPlayerHouseLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the current player house")
             .WithReturnValue("houseName", [](LuaVariableInfo& p) {
                p.WithDescription("The house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("getPlayerBaseHouse", GetPlayerBaseHouseLua, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the name of the base house for the current player house. Every house has a base house which it acts like, usually NOD or GDI.")
             .WithReturnValue("baseHouseName", [](LuaVariableInfo& p) {
                p.WithDescription("The base house name of the player (BADGUY, GOODGUY etc.)")
                 .WithType(LuaType::String);
             });
        });

        WithFunction("getActiveHouses", Lua_Get_Active_Houses, [](LuaFunctionInfo& f) {
            f.WithDescription("Get all houses that are active in the current scenario")
             .WithReturnValue("activeHouseNames", [](LuaVariableInfo& p) {
                p.WithDescription("List of strings, containing the names of active houses.")
                 .WithType(LuaType::Table);
             });
        });

        WithFunction("modifyHouseCredits", ModifiyHouseCreditsLua, [](LuaFunctionInfo& f) {
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

        WithFunction("enableSuperweaponForHouse", EnableHouseSuperWeaponLua, [](LuaFunctionInfo& f) {
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

        WithFunction("chargeSuperweaponForHouse", ChargeHouseSuperWeaponLua, [](LuaFunctionInfo& f) {
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

        WithFunction("disableSuperweaponForHouse", DisableHouseSuperWeaponLua, [](LuaFunctionInfo& f) {
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

        WithFunction("clearHouseMessages", ClearHouseMessagesLua, [](LuaFunctionInfo& f) {
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
