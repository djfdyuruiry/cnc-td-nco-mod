#include <strings.h>

#include "TiberianDawnNcoRuntime.h"

static void Print_Lua_Output(HANDLE stdOut, const char* output)
{
	auto length = strlen(output);

	if (!WriteConsole(stdOut, output, length, NULL, NULL))
	{
		Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

		exit(1);
	}
}

static void Print_Lua_Result(HANDLE stdOut)
{
	char* output;
	auto& luaState = NcoLuaRuntime().GetState();
	auto valueType = luaState.GetType(-1);
	auto value = luaState.ToString(-1);

	// output return value of string or typename if the
	// value was not a string/number
	if (value != NULL)
	{
		if (Strings_Are_Equal(valueType, "boolean"))
		{
			output = FormatString("%s\r\n", Strings_Are_Equal(value, "TRUE") ? "true" : "false");
		}
		else
		{
			output = FormatString("%s\r\n", value);
		}
	}
	else
	{
		output = FormatString("<%s>\r\n", valueType);
	}

	if (output != NULL)
	{
		Print_Lua_Output(stdOut, output);

		delete output;
	}
}

static char* Read_Lua_String(HANDLE stdOut, HANDLE stdIn)
{
	DWORD charsRead;

	// reset all C and Lua variables
	auto input = Allocate_String(2048);

	NcoLuaRuntime().GetState().ClearStack();

	// read input
	if (!WriteConsole(stdOut, "> ", 2, NULL, NULL)
		|| !ReadConsole(stdIn, input, 2048, &charsRead, NULL))
	{
		Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

		exit(1);
	}

	return input;
}

bool Enter_Lua_Repl(bool initialiseLua = true)
{
	if (initialiseLua && !TiberianDawnNcoRuntime::GetInstance().LuaInitWasSuccessful())
	{
		puts("ERROR: Failed to initialise Lua state");
		return false;
	}

	auto stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	auto stdIn = GetStdHandle(STD_INPUT_HANDLE);

	char* input = NULL;

	Toggle_Console_Logging();
	system("cls");

	puts("=======================");
	puts("  NCO Mod Lua Console  ");
	puts("=======================");
	printf("\nHelp: https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki/09.-Lua-Scripting-API\n\n");

	while (true)
	{
		if (input != NULL)
		{
			delete input;
		}

		input = Read_Lua_String(stdOut, stdIn);

		if (Strings_Are_Equal(input, "exit\r\n"))
		{
			break;
		}

		if (Strings_Are_Equal(input, "clear\r\n"))
		{
			system("cls");
			continue;
		}

		if (String_Is_Empty(input))
		{
			continue;
		}
		
		auto& executeResult = NcoLuaRuntime().ExecuteScript(input);
		
		// evaluate lua string
		if (executeResult.IsErrorResult())
		{
			// TODO: remove or cope with newline in GetError result string (currently missing from output)
			auto errorOutput = FormatString("%s\r\n", executeResult.GetError());

			Print_Lua_Output(stdOut, errorOutput);

			delete errorOutput;
			delete &executeResult;

			continue;
		}
		
		delete &executeResult;

		Print_Lua_Result(stdOut);
	}

	Toggle_Console_Logging();

	return true;
}
