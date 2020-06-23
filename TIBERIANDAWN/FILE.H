#ifndef FILE__H
#define FILE__H

#include <Windows.h>

HANDLE Open_File_For_Appending(const char* filename);

void Append_To_File(HANDLE file, char* data);

#endif
