#include "Director.h"

void Director::startWork()
{
	timer_->startWork();
	for (auto i : house)
	{
		i->startWork();
	}
}

void Director::stopWork()
{
	for (auto i : house)
	{
		i->stopWork();
	}
	timer_->stopWork();
}

double Director::getPower(int number)
{
	if (number >= house.size())
		return 0;
	return house[number]->checkData();
}

double Director::getTime()
{
	return timer_->currentTime();
}