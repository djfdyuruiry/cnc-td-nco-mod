#ifdef TEST_CONSOLE

#include "function.h"

static void Print_Lua_Result(HANDLE stdOut)
{
	auto output = Allocate_String(2048);
	auto valueType = Get_Lua_Value_Type(-1);
	auto value = Get_Lua_Value_As_String(-1);

	// output return value of string or typename if the
	// value was not a string/number
	if (value != NULL)
	{
		if (Strings_Are_Equal(valueType, "boolean"))
		{
			sprintf(output, "%s\r\n", Strings_Are_Equal(value, "TRUE") ? "true" : "false");
		}
		else
		{
			sprintf(output, "%s\r\n", value);
		}
	}
	else
	{
		sprintf(output, "<%s>\r\n", valueType);
	}

	if (!WriteConsole(stdOut, output, strlen(output), NULL, NULL))
	{
		Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

		exit(1);
	}

	delete output;
}

static char* Read_Lua_String(HANDLE stdOut, HANDLE stdIn)
{
	DWORD charsRead;

	// reset all C and Lua variables
	auto input = Allocate_String(2048);

	Clear_Lua_Stack();

	// read input
	if (!WriteConsole(stdOut, "> ", 2, NULL, NULL)
		|| !ReadConsole(stdIn, input, 2048, &charsRead, NULL))
	{
		Log_Error("Lua REPL error: %s", Get_Win32_Error_Message());

		exit(1);
	}

	return input;
}

void Enter_Lua_Repl()
{
	if (!Initialise_Lua())
	{
		puts("ERROR: Failed to initialise Lua state");
		exit(1);
	}

	auto stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	auto stdIn = GetStdHandle(STD_INPUT_HANDLE);

	char* input = NULL;

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

		// evaluate lua string
		if (!Execute_Lua_Script(input, true))
		{
			continue;
		}

		Print_Lua_Result(stdOut);
	}
}

#endif
