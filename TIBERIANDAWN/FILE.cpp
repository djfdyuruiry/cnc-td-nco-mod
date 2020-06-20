#include "function.h"

void Append_To_File(const char* filename, char* data)
{
    auto hFile = CreateFile(
        filename,  // name of the write
        FILE_APPEND_DATA,          // open for appending
        FILE_SHARE_READ,           // share for reading only
        NULL,                      // default security
        OPEN_ALWAYS,               // open existing file or create new file 
        FILE_ATTRIBUTE_NORMAL,     // normal file
        NULL
    );                     // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    auto dwBytesToWrite = strlen(data);

    WriteFile(
        hFile,              // open file handle
        data,               // start of data to write
        dwBytesToWrite,     // number of bytes to write
        NULL,    // number of bytes that were written
        NULL
    );              // no overlapped structure

    CloseHandle(hFile);
}
