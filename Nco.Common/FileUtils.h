#pragma once

#include <windows.h>

class FileUtils final
{
private:
	FileUtils()
	{
	}

public:
	static bool IsFile(const char* path)
	{
		auto fileInfo = GetFileAttributes(path);

		return fileInfo != INVALID_FILE_ATTRIBUTES
			&& !(fileInfo & FILE_ATTRIBUTE_DIRECTORY);
	}
};
