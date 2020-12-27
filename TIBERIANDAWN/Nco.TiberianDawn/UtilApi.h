#pragma once

#include <iostream>
#include <vector>

#include <ILuaStateWrapper.h>
#include <Logger.h>
#include <LuaFunctionInfo.h>
#include <LuaVariableInfo.h>
#include <strings.h>
#include <utils.h>

#include "TiberianDawnNcoRuntime.h"

class UtilApi : public LuaApi
{
private:
    static int Lua_BuildModDataFilePath(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        if (!LuaObjectUtils::ValidateCurrentFunctionParameters(luaState))
        {
            return 0;
        }

        auto& filePathResult = luaState.ReadString(1);

        if (StringIsEmpty(filePathResult.GetValue()))
        {
            luaState.RaiseError("buildModDataFilePath parameter `filePath` was blank");
        
            delete &filePathResult;
        
            return 0;
        }

        luaState.WriteString(
            BuildModDataFilePath(filePathResult.GetValue())
        );
    
        delete &filePathResult;

        return 1;
    }

    static int Lua_GetModDataPath(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        luaState.WriteString(GetModDataPath());

        return 1;
    }

    static int Lua_GetNowInEpochMillis(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        luaState.WriteBigInteger(GetNowInEpochMillis());

        return 1;
    }

    static int Lua_Toggle_Console_Logging(lua_State* _)
    {
        GetLogger().ToggleConsoleLogging();

        return 0;
    }

    static int Lua_Set_Log_Level(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        if (!LuaObjectUtils::ValidateCurrentFunctionParameters(luaState))
        {
            return 0;
        }

        auto& logLevelStrResult = luaState.ReadString(1);

        if (StringIsEmpty(logLevelStrResult.GetValue()))
        {
            luaState.RaiseError("setLogLevel parameter `logLevel` was blank");

            delete &logLevelStrResult;

            return 0;
        }

        auto logLevelStr = logLevelStrResult.GetValue();
        auto logLevelUpper = ConvertStringToUpperCase(logLevelStr);

        auto logLevel = ParseLogLevel(logLevelUpper);

        delete logLevelUpper;

        GetLogger().SetLogLevel(logLevel);

        delete &logLevelStrResult;

        return 0;
    }

    static int Lua_Get_Log_Level(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto logLevel = LogLevelToString(GetLogger().GetLogLevel());

        luaState.WriteString(logLevel);

        return 1;
    }

    static int Lua_ShowError(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();

        if (!LuaObjectUtils::ValidateCurrentFunctionParameters(luaState))
        {
            return 0;
        }

        auto& messageResult = luaState.ReadString(1);

        if (StringIsEmpty(messageResult.GetValue()))
        {
            luaState.RaiseError("showError parameter `message` was blank");

            delete &messageResult;

            return 0;
        }

        ShowError(messageResult.GetValue());

        delete &messageResult;

        return 0;
    }

    int ReadString(ILuaStateWrapper& lua)
    {
        auto stdIn = GetStdHandle(STD_INPUT_HANDLE);

        if (!Win32HandleIsValid(stdIn))
        {
            lua.RaiseError("Failed to open standard input stream");

            return 0;
        }

        auto input = AllocateString(2048);

        // read input
        DWORD charsRead;

        if (!ReadConsole(stdIn, input, 2048, &charsRead, NULL))
        {
            WithWin32ErrorMessage([&](auto e) {
                lua.RaiseError("Error reading from standard input: %s", e);
            });

            CloseWin32HandleIfValid(stdIn);

            return 0;
        }

        lua.WriteString(input);

        return 1;
    }

    static int LuaReadString(lua_State* lua)
    {
        return LUA_METHOD_PROXY(UtilApi, ReadString);
    }

    static int Lua_Log(lua_State* _)
    {
        auto& luaState = NcoLuaRuntime().GetState();
        auto argCount = luaState.GetStackTop();

        if (argCount > 0)
        {
            auto& logMessageResult = luaState.ReadString(1);

            if (!logMessageResult.IsErrorResult())
            {
                LogInfo("Lua => %s", logMessageResult.GetValue());
            }

            delete &logMessageResult;
        }

        return 0;
    }

    UtilApi()
    {
        WithName("Utils");

        WithDescription("Utility functions");

        WithFunction("logString", Lua_Log, [](LuaFunctionInfo& f) {
            f.WithDescription("Write a info line to the log file")
             .WithParameter("str", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });

        WithMethod("readString", this, LuaReadString, [](LuaFunctionInfo& f) {
            f.WithDescription("Read a line from standard input")
             .WithReturnValue("str", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });

        WithFunction("showErrorString", Lua_ShowError, [](LuaFunctionInfo& f) {
            f.WithDescription("Display an error to the user in a message box")
             .WithParameter("errorString", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });
         
        WithFunction("getLogLevel", Lua_Get_Log_Level, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the current log level")
             .WithReturnValue("logLevel", [](LuaVariableInfo& p) {
                p.WithDescription("See the logging config for valid values")
                 .WithType(LuaType::String);
             });
        });
         
        WithFunction("setLogLevel", Lua_Set_Log_Level, [](LuaFunctionInfo& f) {
            f.WithDescription("Set the current log level")
             .WithParameter("logLevel", [](LuaVariableInfo& p) {
                p.WithDescription("See the logging config for valid values")
                 .WithType(LuaType::String);
             });
        });
         
        WithFunction("toggleConsoleLog", Lua_Toggle_Console_Logging, [](LuaFunctionInfo& f) {
            f.WithDescription("Toggle logging to the console on/off");
        });

        WithFunction("getNowInEpochMillis", Lua_GetNowInEpochMillis, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the number of milliseconds between unix epoch and now")
             .WithReturnValue("nowInEpochMillis", [](LuaVariableInfo& p) {
                p.WithType(LuaType::Number);
             });
        });

        WithFunction("getModDataPath", Lua_GetModDataPath, [](LuaFunctionInfo& f) {
            f.WithDescription("Get the absolute path to the mod data directory")
             .WithReturnValue("modDataDirPath", [](LuaVariableInfo& p) {
                p.WithType(LuaType::String);
             });
        });

        WithFunction("buildModDataFilePath", Lua_BuildModDataFilePath, [](LuaFunctionInfo& f) {
            f.WithDescription("Build the absolute path to a file in the mod data directory")
             .WithParameter("filename", [](LuaVariableInfo& p) {
                p.WithDescription("Mod data filename")
                 .WithType(LuaType::String);
             })
             .WithReturnValue("filePath", [](LuaVariableInfo& p) {
                 p.WithDescription("Absolute file path for given filename")
                  .WithType(LuaType::String);
              });
        });
    }

public:
    static ILuaApi& Build()
    {
        return *(new UtilApi());
    }

};
