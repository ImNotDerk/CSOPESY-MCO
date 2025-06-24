#pragma once
#include "ICommand.h"
#include "IETThread.h"
#include "ConsoleManager.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, String& toPrint);
	void execute() override;
private:
	String toPrint;
};

