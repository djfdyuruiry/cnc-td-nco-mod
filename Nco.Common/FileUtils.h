#pragma once

#include <windows.h>

#include "Logger.h"
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
		auto handle = (HANDLE)OpenFile(path, &ofstruct, readOnly ? OF_READ : OF_READWRITE);

		auto fileOpen = Win32HandleIsValid(handle);

		if (fileOpen)
		{
			*fileHandle = handle;
		}
		else
		{
			With_Win32_Error_Message([&](auto e) {
				LogError("Failed to open file '%s': %s", path, e);
			});
		}

		return fileOpen;
	}


	static HANDLE OpenFileForAppending(const char* filename, bool* errorOccured)
	{
		if (String_Is_Empty(filename))
		{
			Show_Error("Filename passed to OpenFileForAppending was null or empty");

			return NULL;
		}

		auto file = CreateFile(
			filename,
			FILE_APPEND_DATA,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (!Win32HandleIsValid(file))
		{
			*errorOccured = true;

			return NULL;
		}

		return file;
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
			With_Win32_Error_Message([&] (auto e) {
				LogError("Failed to read text from file '%s': %s", path, e);
			});

			delete data;

			return NULL;
		}

		return data;
	}

	static void AppendTextToFile(HANDLE file, char* data)
	{
		if (String_Is_Empty(data))
		{
			return;
		}

		if (!Win32HandleIsValid(file))
		{
			Show_Error("File handle passed to WriteTextToFile was invalid");
			return;
		}

		DWORD written;

		// TODO: consider dealing with return val
		WriteFile(
			file,
			data,
			strlen(data),
			&written,
			NULL
		);
	}

};
