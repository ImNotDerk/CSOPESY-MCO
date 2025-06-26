#pragma once
#include "ICommand.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, String& toPrint);
	void execute() override;
	String getOutput() const override;
private:
	String toPrint;
};

