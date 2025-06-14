#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName)
{
	this->schedulingAlgo = schedulingAlgo;
	this->pid = pid;
	this->processName = processName;
}

std::shared_ptr<Process> AScheduler::findProcess()
{
	for (auto process : this->processes)
	{
		if (process->getName() == this->processName)
		{
			return process;
		}
	}
}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	return std::shared_ptr<Process>();
}

void AScheduler::run()
{
	this->init();
	while (true) 
	{
		this->execute();
	}
}

void AScheduler::stop()
{
	this->running = false;
}
