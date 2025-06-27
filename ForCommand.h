#pragma once
#include "ICommand.h"
#include <vector>

class ForCommand : public ICommand
{
public:
	ForCommand(int processID, std::vector<ICommand*> instructions, int repeats)
		: ICommand(processID, CommandType::FOR)
	{
		this->processID = processID;
		this->instructions = instructions;
		this->repeats = repeats;
	}

	void performForCommand();
	void execute() override;

private:
	int processID;
	std::vector<ICommand*> instructions;
	int repeats;
};