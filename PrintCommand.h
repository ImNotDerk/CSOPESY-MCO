#pragma once
#include "ICommand.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, String& toPrint);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;
	String getOutput() const override;
private:
	String toPrint;
};

