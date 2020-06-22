#include "function.h"

/// <summary>
/// Log an error then show it in a user message box.
/// </summary>
/// <param name="messageFormat">Message in <ref>sprintf</ref> format.</param>
/// <param name="..."><ref>sprintf</ref> format arguments</param>
void Show_Error(const char* messageFormat, ...)
{
	auto logLineLength = Get_Log_Line_Length();
	auto formattedMessageBuffer = new char[logLineLength];

	va_list formatArgs;
	va_start(formatArgs, messageFormat);

	vsnprintf(formattedMessageBuffer, logLineLength, messageFormat, formatArgs);

	va_end(formatArgs);

	Log_Error(formattedMessageBuffer);

	MessageBox(
		NULL,
		formattedMessageBuffer,
		"Command & Conquer",
		MB_OK | MB_ICONSTOP
	);

	delete formattedMessageBuffer;
}
