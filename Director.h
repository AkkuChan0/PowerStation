#pragma once

#include "Object.h"
#include <vector>


class Director
{
	Station* timer_ = new Station;
	std::vector<Object*> house;

public:
	Director() : house({ new Object(100, 20, *timer_), new Object(180, 50, *timer_), new Object(90, 20, *timer_) }) {}
	void startWork();
	void stopWork();
	double getPower(int number);
	double getTime();
};