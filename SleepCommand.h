#pragma once
#include "ICommand.h"
class SleepCommand : public ICommand
{
public:
	SleepCommand(int processID, uint8_t sleepTicks) : ICommand(processID, CommandType::SLEEP) 
	{
		this->processID = processID;
		this->sleepTicks = sleepTicks;
	}

	void execute() override;

private:
	int processID; 
	uint8_t sleepTicks;
};

