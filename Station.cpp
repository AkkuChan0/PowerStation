#include "Station.h"

Station::Station()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);

	m_time = 0;

	hStopEvt = CreateEvent(NULL, TRUE, TRUE, NULL);

	hThread = INVALID_HANDLE_VALUE;
}

Station::~Station()
{
	CloseHandle(m_hMutex);
	SetEvent(hStopEvt);

	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		TerminateThread(hThread, 1);
	}
}

void Station::startWork()
{
	if (hThread != INVALID_HANDLE_VALUE) return;
	ResetEvent(hStopEvt);
	hThread = CreateThread(NULL, 0, timeThread, this, 0, NULL);
}

void Station::stopWork()
{
	if (hThread == INVALID_HANDLE_VALUE) return;
	SetEvent(hStopEvt);

	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;
}

void Station::changeTime()
{
	MutexLocker guard(m_hMutex);
	if (m_time >= 23)
		m_time = 0;
	else
		++m_time;
}

DWORD WINAPI Station::timeThread(LPVOID ptr)
{
	Station* _this = (Station*)ptr;
	for (;;)
	{
		Sleep(1000);
		_this->changeTime();
		if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT)
			break;
	}

	return 0;
}

double Station::currentTime()
{
	MutexLocker guard(m_hMutex);
	return m_time;
}