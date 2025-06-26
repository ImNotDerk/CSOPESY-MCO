#include "PrintCommand.h"
#include "IETThread.h"
//#include "MessageBuffer.h"

PrintCommand::PrintCommand(int processID, String& toPrint) : ICommand(processID, PRINT)
{
	this->toPrint = toPrint;
}

void PrintCommand::execute()
{
	ICommand::execute();
}

String PrintCommand::getOutput() const
{
	return toPrint;
}