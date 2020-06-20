#include "function.h"

static auto LOG_LINE_LENGTH = 1024;

/// <summary>
/// Log an error, show it in a user message box and exit the game
/// </summary>
/// <param name="messageFormat">Message in <ref>sprintf</ref> format.</param>
/// <param name="..."><ref>sprintf</ref> format arguments</param>
void Error_And_Exit(char* messageFormat, ...)
{
	auto formattedMessageBuffer = new char[LOG_LINE_LENGTH];

	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	vsnprintf(formattedMessageBuffer, LOG_LINE_LENGTH, messageFormat, formatArgs);

	va_end(formatArgs);

	Log_Error(formattedMessageBuffer);

	MessageBox(
		NULL,
		formattedMessageBuffer,
		"Command & Conquer",
		MB_OK | MB_ICONSTOP
	);

	delete formattedMessageBuffer;

	exit(1);
}
