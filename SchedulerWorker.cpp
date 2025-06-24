#include "SchedulerWorker.h"
//#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (isRunning) {
		//GlobalScheduler::getInstance()->tick();
	}
}