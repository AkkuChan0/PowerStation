#pragma once

#include <windows.h>

class MutexLocker
{
	HANDLE m_Mutex;

public:
	MutexLocker(HANDLE mutex) : m_Mutex(mutex)
	{
		WaitForSingleObject(m_Mutex, INFINITE);
	}

	~MutexLocker()
	{
		ReleaseMutex(m_Mutex);
	}
};

class Station
{
	double m_time;

public:

	Station();
	~Station();

	double currentTime();

	void startWork();
	void stopWork();

private:

	HANDLE hStopEvt;
	HANDLE m_hMutex;
	HANDLE hThread;

	void changeTime();
	static DWORD WINAPI timeThread(LPVOID ptr);
};