#include "PrintCommand.h"

PrintCommand::PrintCommand(int processID, String& toPrint) : ICommand(processID, PRINT)
{
	this->toPrint = toPrint;
}

void PrintCommand::execute()
{
	ICommand::execute();

	std::cout << toPrint << std::endl;
}
