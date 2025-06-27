#pragma once
#include "ICommand.h"
#include "Process.h"
#include <vector>

class ForCommand : public ICommand
{
public:
	ForCommand(int processID, std::vector<ICommand*> instructions, int repeats) : ICommand(processID, CommandType::FOR)
	{
		this->processID = processID;
		this->instructions = instructions;
		this->repeats = repeats;
	}
	void performForCommand();
	void performWithLogging(class Process* process, int coreId, int currentDepth = 1);
	void execute() override;

private:
	int processID;
	std::vector<ICommand*> instructions;
	int repeats;
};

