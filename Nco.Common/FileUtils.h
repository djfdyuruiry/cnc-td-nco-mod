#pragma once

#include <windows.h>

#include "logger.h"
#include "strings.h"
#include "utils.h"

class FileUtils final
{
private:
	FileUtils()
	{
	}

public:
	static bool IsFile(const char* path)
	{
		if (String_Is_Empty(path))
		{
			return false;
		}

		auto fileInfo = GetFileAttributes(path);

		return fileInfo != INVALID_FILE_ATTRIBUTES
			&& !(fileInfo & FILE_ATTRIBUTE_DIRECTORY);
	}

	static bool IsDirectory(const char* path)
	{
		if (String_Is_Empty(path))
		{
			return false;
		}

		auto fileInfo = GetFileAttributes(path);

		return fileInfo != INVALID_FILE_ATTRIBUTES
			&& fileInfo & FILE_ATTRIBUTE_DIRECTORY;
	}

	static bool CreateFileHandle(const char* path, HANDLE* fileHandle, bool readOnly)
	{
		OFSTRUCT ofstruct;
		*fileHandle = (HANDLE)OpenFile(path, &ofstruct, readOnly ? OF_READ : OF_READWRITE);

		if (*fileHandle == INVALID_HANDLE_VALUE)
		{
			auto error = Get_Win32_Error_Message();

			Log_Error("Failed to open file '%s': %s", path, error);

			delete error;

			return false;
		}

		return true;
	}

	static char* ReadFileText(const char* path)
	{
		HANDLE fileHandle;

		if (String_Is_Empty(path) || !CreateFileHandle(path, &fileHandle, true))
		{
			return NULL;
		}

		DWORD fileSize;

		fileSize = GetFileSize(fileHandle, &fileSize);

		auto data = Allocate_String(fileSize);

		if (!ReadFile(fileHandle, data, fileSize, NULL, NULL))
		{
			auto error = Get_Win32_Error_Message();

			Log_Error("Failed to read text from file '%s': %s", path, error);

			delete data;
			delete error;

			return NULL;
		}

		return data;
	}
};
