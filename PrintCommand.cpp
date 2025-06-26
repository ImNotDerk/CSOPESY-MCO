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

	//std::cout << this->toPrint << std::endl;
	//std::stringstream msg; msg << String("PID ") << this->processID << ":" << this->toPrint; << std::endl;
	//MessageBuffer::log(msg.str());
}
