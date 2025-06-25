#include "SleepCommand.h"

void SleepCommand::execute()
{
	ICommand::execute();

	IETThread::sleep(this->sleepTicks);
	std::cout << "Process " << this->processID << " has slept for " << this->sleepTicks << " ticks." << std::endl;
}