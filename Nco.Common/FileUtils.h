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
		if (StringIsEmpty(path))
		{
			return false;
		}

		auto fileInfo = GetFileAttributes(path);

		return fileInfo != INVALID_FILE_ATTRIBUTES
			&& !(fileInfo & FILE_ATTRIBUTE_DIRECTORY);
	}

	static bool IsDirectory(const char* path)
	{
		if (StringIsEmpty(path))
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
			WithWin32ErrorMessage([&](auto e) {
				LogError("Failed to open file '%s': %s", path, e);
			});
		}

		return fileOpen;
	}


	static HANDLE OpenFileForAppending(const char* filename, bool* errorOccured)
	{
		if (StringIsEmpty(filename))
		{
			ShowError("Filename passed to OpenFileForAppending was null or empty");

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

		if (StringIsEmpty(path) || !CreateFileHandle(path, &fileHandle, true))
		{
			return NULL;
		}

		DWORD fileSize;

		fileSize = GetFileSize(fileHandle, &fileSize);

		auto data = AllocateString(fileSize);

		if (!ReadFile(fileHandle, data, fileSize, NULL, NULL))
		{
			WithWin32ErrorMessage([&] (auto e) {
				LogError("Failed to read text from file '%s': %s", path, e);
			});

			delete data;

			return NULL;
		}

		return data;
	}

	static void AppendTextToFile(HANDLE file, char* data)
	{
		if (StringIsEmpty(data))
		{
			return;
		}

		if (!Win32HandleIsValid(file))
		{
			ShowError("File handle passed to AppendTextToFile was invalid");
			return;
		}

		DWORD written;

		auto writeStatus = WriteFile(
			file,
			data,
			strlen(data),
			&written,
			NULL
		);

		if (!writeStatus)
		{
			WithWin32ErrorMessage([&](auto e)
			{
				ShowError("Failed to write text to file: %s", e);
			});
		}
	}

};
