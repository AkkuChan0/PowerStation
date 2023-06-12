#pragma once

#include "Station.h"

class Object
{
	double a_, b_;
	Station& timer_;
	double m_data;
	HANDLE hStopEvt;
	HANDLE m_hMutex;

	HANDLE hThread;

public:

	Object(double a, double b, Station& timer);
	~Object();

	void changeObject(double time);
	double checkData();

	void startWork();
	void stopWork();

	static DWORD WINAPI changeGen(LPVOID ptr);
};