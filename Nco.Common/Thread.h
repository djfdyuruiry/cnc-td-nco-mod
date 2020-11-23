#pragma once

#include <stdlib.h>
#include <Windows.h>

#include "Logger.h"
#include "utils.h"

class Thread
{
private:
	static DWORD WINAPI RunProxy(LPVOID lpParam)
	{
		auto thread = (Thread*)lpParam;

		return thread->Run();
	}

protected:
	const char* name;
	bool threadRunning;
	HANDLE threadHandle;

	virtual DWORD Run() = 0;

	Thread(const char* name) : name(name), threadRunning(false), threadHandle(NULL)
	{
	}

public:
	~Thread()
	{
		if (threadRunning)
		{
			Stop();
		}
	}

	const char* GetName()
	{
		return name;
	}

	bool Start()
	{
		if (threadRunning)
		{
			LogError("Attempted to start thread that is already running: %s", name);

			return false;
		}

		threadHandle = CreateThread(
			NULL,
			0,
			&RunProxy,
			this,
			NULL,
			NULL
		);

		threadRunning = threadHandle != NULL && threadHandle != INVALID_HANDLE_VALUE;

		if (!threadRunning)
		{
			WithWin32ErrorMessage([&] (auto e) {
				LogError("Failed to start thread '%s': %s", name, e);
			});

			threadHandle = NULL;
		}
		else
		{
			LogDebug("Thread started: %s", name);
		}

		return threadRunning;
	}

	bool Stop()
	{
		if (!threadRunning)
		{
			LogError("Attempted to stop thread that is not running: %s", name);

			return false;
		}

		threadRunning = !TerminateThread(threadHandle, 0);

		if (threadRunning)
		{
			WithWin32ErrorMessage([&] (auto e) {
				LogError("Failed to stop thread '%s': %s", name, e);
			});

			return false;
		}
		else
		{
			LogDebug("Thread stopped: %s", name);
		}

		return !threadRunning;
	}
};
