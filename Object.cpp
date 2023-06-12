#include "Object.h"
#include <cmath>

#define M_PI 3.14159265359

/*
������� (�����������)
*/

Object::Object(double a, double b, Station& timer) : timer_(timer)
{
	hStopEvt = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hMutex = CreateMutex(NULL, FALSE, NULL);

	a_ = a;
	b_ = b;

	m_data = 0;
	hThread = INVALID_HANDLE_VALUE;
}

Object::~Object()
{
	stopWork();
	CloseHandle(hStopEvt);
	CloseHandle(m_hMutex);
}

void Object::changeObject(double time)
{
	MutexLocker guard(m_hMutex);
	m_data = a_ + b_ * sin(M_PI / 2 * (time / 6 - 1));
}

double Object::checkData()
{
	MutexLocker guard(m_hMutex);
	return m_data;
}

void Object::startWork()
{
	if (hThread != INVALID_HANDLE_VALUE) return;
	ResetEvent(hStopEvt);
	hThread = CreateThread(NULL, 0, changeGen, this, 0, NULL);
}

void Object::stopWork()
{
	if (hThread == INVALID_HANDLE_VALUE) return; //�� �������
	SetEvent(hStopEvt); //������� ���� ���������
	//����, ����� ����� ������� ��� ���� � �����������
	if (WaitForSingleObject(hThread, 5000) != WAIT_OBJECT_0)
	{
		//�������� ���� �����, �� �����������, "��������" ������, ����������, ����� �� �������
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;
}

DWORD WINAPI Object::changeGen(LPVOID ptr)
{
	Object* _this = (Object*)ptr;
	if (!_this) return 1; //������ ��� ���� ������, �� ����� ��������

	for (;;)
	{
		_this->changeObject(_this->timer_.currentTime());
		Sleep(1000);
		//������, ����� �� ���� ����� ������
		if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT)
			break;
	}
	return 0;
}