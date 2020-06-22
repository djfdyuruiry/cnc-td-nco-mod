#include "function.h"

HANDLE Open_File_For_Appending(const char* filename)
{
    auto file = CreateFile(
        filename,  // name of the write
        FILE_APPEND_DATA,          // open for appending
        FILE_SHARE_READ,           // share for reading only
        NULL,                      // default security
        OPEN_ALWAYS,               // open existing file or create new file 
        FILE_ATTRIBUTE_NORMAL,     // normal file
        NULL
    );

    if (file == INVALID_HANDLE_VALUE || file == NULL)
    {
        return NULL;
    }

    return file;
}

void Append_To_File(HANDLE file, char* data)
{
    if (file == NULL || file == INVALID_HANDLE_VALUE)
    {
        return;
    }

    auto dwBytesToWrite = strlen(data);

    WriteFile(
        file,              // open file handle
        data,               // start of data to write
        dwBytesToWrite,     // number of bytes to write
        NULL,    // number of bytes that were written
        NULL
    );              // no overlapped structure
}
