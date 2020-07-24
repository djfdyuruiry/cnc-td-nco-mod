#include "function.h"

static HousesType Lua_Parse_House_Type(lua_State* lua, int index, const char* callingFunctionName, bool* parseError)
{
    auto houseTypeString = Get_Lua_Value_As_String(index);
    auto upperHouseTypeString = Convert_String_To_Upper_Case(houseTypeString);
    auto houseType = Parse_House_Type(upperHouseTypeString, parseError);

    delete upperHouseTypeString;

    if (*parseError)
    {
        luaL_error(lua, "%s parameter `houseName` was not a valid house: %s", callingFunctionName, houseTypeString);

        return HOUSE_NONE;
    }

    return houseType;
}

static int Lua_Clear_House_Messages(lua_State* lua)
{
    Log_Trace("Lua_Clear_House_Messages called from Lua");

    auto argCount = lua_gettop(lua);

    if (argCount < 1)
    {
        luaL_error(lua, "clearHouseMessages requires at least one argument");

        return 0;
    }

    bool parseError = false;
    auto house = Lua_Parse_House_Type(lua, 1, "clearHouseMessages", &parseError);

    if (parseError)
    {
        return 0;
    }

    Clear_House_Messages(house);

    return 0;
}

static int Lua_Clear_Game_Loop_Messages(lua_State* lua)
{
    Clear_Game_Loop_Messages();

    return 0;
}

static int Lua_Clear_Game_Ui_Messages(lua_State* lua)
{
    Clear_Game_Ui_Messages();

    return 0;
}


static HouseClass* Get_House_By_Type(HousesType houseType, bool* housePresent)
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

static SuperweaponType Lua_Parse_Superweapon_Type(lua_State* lua, int index, const char* callingFunctionName, bool* invalidValue)
{
    auto superWeaponName = Get_Lua_Value_As_String(index);

    if (String_Is_Empty(superWeaponName))
    {
        luaL_error(lua, "%s parameter `superWeaponName` was nil or empty", callingFunctionName);

        if (invalidValue != NULL)
        {
            *invalidValue = true;
        }

        return NO_SUPERWEAPON;
    }

    auto weapon = NO_SUPERWEAPON;
    auto uppercaseName = Convert_String_To_Upper_Case(superWeaponName);

    if (Strings_Are_Equal(uppercaseName, AIRSTRIKE_SECTION_NAME_UPPER))
    {
        weapon = AIRSTRIKE;
    }
    else if (Strings_Are_Equal(uppercaseName, ION_CANNON_SECTION_NAME_UPPER))
    {
        weapon = ION_CANNON;
    }
    else if (Strings_Are_Equal(uppercaseName, NUCLEAR_STRIKE_SECTION_NAME_UPPER))
    {
        weapon = NUCLEAR_STRIKE;
    }
    else
    {
        luaL_error(lua, "%s parameter `superWeaponName` was not recognised as a superweapon name: %s", callingFunctionName, superWeaponName);
    }

    delete uppercaseName;

    return weapon;
}

static int Lua_Send_House_Super_Weapon_Message(
    lua_State* lua,
    const char* callingFunctionName,
    const char* callingLuaFunctionName,
    SuperweaponMessageType messageType
) {
    Log_Trace("%s called from Lua", callingFunctionName);

    auto argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "%s requires at least two arguments", callingLuaFunctionName);
        return 0;
    }

    bool parseError = false;
    auto house = Lua_Parse_House_Type(lua, 1, callingLuaFunctionName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto superweapon = Lua_Parse_Superweapon_Type(lua, 2, callingLuaFunctionName, &parseError);

    if (parseError)
    {
        return 0;
    }

    auto oneTime = false;

    if (argCount > 2)
    {
        if (!lua_isboolean(lua, 3))
        {
            luaL_error(lua, "%s parameter `oneTimeOnly` must be a boolean", callingLuaFunctionName);
            return 0;
        }

        oneTime = lua_toboolean(lua, 3);
    }

    HouseMessage message{};

    message.oneTime = oneTime;
    message.superweapon = superweapon;
    message.superweaponMessage = messageType;

    Push_House_Message(house, SUPERWEAPON_MESSAGE, message);

    return 0;
}

static int Lua_Disable_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "disableSuperWeaponForHouse",
        DISABLE_SUPERWEAPON
    );
}

static int Lua_Charge_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "chargeSuperWeaponForHouse",
        CHARGE_SUPERWEAPON
    );
}

static int Lua_Enable_House_Super_Weapon(lua_State* lua) {
    return Lua_Send_House_Super_Weapon_Message(
        lua,
        "Lua_Enable_House_Super_Weapon_Message",
        "enableSuperWeaponForHouse",
        ENABLE_SUPERWEAPON
    );
}

static int Lua_Modifiy_House_Credits(lua_State* lua)
{
    Log_Trace("Lua_Modify_House_Credits called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "modifyHouseCredits requires exactly two arguments");
        return 0;
    }

    bool invalidValue = false;
    auto houseType = Lua_Parse_House_Type(lua, 1, "modifyHouseCredits", &invalidValue);

    if (invalidValue)
    {
        return 0;
    }

    if (!lua_isnumber(lua, 2))
    {
        luaL_error(lua, "modifyHouseCredits parmeter `creditsModifier` must be a number");
        return 0;
    }

    auto creditsModifier = luaL_checknumber(lua, 2);
    auto house = Get_House_By_Type(houseType, &invalidValue);

    if (invalidValue)
    {
        Log_Error("Ignoring modifyHouseCredits call as house '%s' is not in the current game", House_Type_To_String(houseType));

        return 0;
    }

    house->Credits += creditsModifier;

    lua_pushnumber(lua, house->Credits);

    return 1;
}

static int Lua_Get_Active_Houses(lua_State* lua)
{
    Log_Trace("Lua_Get_Active_Houses called from Lua");

    auto activeHousesCount = Houses.Count();

    lua_createtable(lua, 0, activeHousesCount);

    for (auto i = 0; i < activeHousesCount; i++)
    {
        auto house = Houses.Raw_Ptr(i);
        auto houseName = House_Type_To_String(house->Class->House);

        lua_pushstring(lua, houseName);
        lua_rawseti(lua, -2, i + 1);
    }

    return 1;
}

static int Lua_Get_Player_Base_House(lua_State* lua)
{
    Log_Trace("Lua_Get_Player_House called from Lua");

    auto playerBaseHouse = House_Type_To_String(PlayerPtr->ActLike);

    lua_pushstring(lua, playerBaseHouse);

    return 1;
}

static int Lua_Get_Player_House(lua_State* lua)
{
    Log_Trace("Lua_Get_Player_House called from Lua");

    auto playerHouse = House_Type_To_String(PlayerPtr->Class->House);

    lua_pushstring(lua, playerHouse);

    return 1;
}

static int Lua_Hide_Map(lua_State* lua)
{
    Log_Trace("Lua_Hide_Map called from Lua");

    Push_Hide_Map_Ui_Message();

    return 0;
}

static int Lua_Reveal_Map(lua_State* lua)
{
    Log_Trace("Lua_Reveal_Map called from Lua");

    Push_Reveal_Map_Ui_Message();

    return 0;
}

static int Lua_Refresh_Map(lua_State* lua)
{
    Log_Trace("Lua_Refresh_Map called from Lua");

    Push_Refresh_Map_Ui_Message();

    return 0;
}

static int Lua_Refresh_Sidebar(lua_State* lua)
{
    Log_Trace("Lua_Refresh_Sidebar called from Lua");

    Push_Refresh_Sidebar_Ui_Message();

    return 0;
}

static int Lua_Show_Game_Message(lua_State* lua)
{
    Log_Trace("Lua_Show_Game_Message called from Lua");

    int argCount = lua_gettop(lua);

    if (argCount < 2)
    {
        luaL_error(lua, "showGameMessage requires exactly two arguments");
        return 0;
    }

    if (!lua_isnumber(lua, 2))
    {
        luaL_error(lua, "showGameMessage parameter `durationInSeconds` must be a number");
        return 0;
    }

    auto message = Get_Lua_Value_As_String(1);
    auto durationInSeconds = luaL_checknumber(lua, 2);

    if (message == NULL)
    {
        luaL_error(lua, "showGameMessage parameter `message` was nil");
        return 0;
    }

    if (durationInSeconds < 0.1)
    {
        luaL_error(lua, "showGameMessage parameter `durationInSeconds` must be equal to or greater than 0.1 (100ms)");
        return 0;
    }

    Push_Show_Notification_Ui_Message(message, durationInSeconds);

    return 0;
}

bool Register_Game_Functions()
{
    Log_Debug("Initialising Lua API game functions");

    Register_Lua_Function("showGameMessage", Lua_Show_Game_Message);

    Register_Lua_Function("refreshSidebar", Lua_Refresh_Sidebar);

    Register_Lua_Function("revealEntireMap", Lua_Reveal_Map);
    Register_Lua_Function("hideEntireMap", Lua_Hide_Map);

    Register_Lua_Function("getPlayerHouse", Lua_Get_Player_House);
    Register_Lua_Function("getPlayerBaseHouse", Lua_Get_Player_Base_House);
    Register_Lua_Function("getActiveHouses", Lua_Get_Active_Houses);

    Register_Lua_Function("modifyHouseCredits", Lua_Modifiy_House_Credits);
    Register_Lua_Function("enableSuperweaponForHouse", Lua_Enable_House_Super_Weapon);
    Register_Lua_Function("chargeSuperweaponForHouse", Lua_Charge_House_Super_Weapon);
    Register_Lua_Function("disableSuperweaponForHouse", Lua_Disable_House_Super_Weapon);

    Register_Lua_Function("clearGameUiMessages", Lua_Clear_Game_Ui_Messages);
    Register_Lua_Function("clearGameLoopMessages", Lua_Clear_Game_Loop_Messages);
    Register_Lua_Function("clearHouseMessages", Lua_Clear_House_Messages);

    return true;
}
