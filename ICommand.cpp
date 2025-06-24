#include "ICommand.h"

ICommand::CommandType ICommand::getCommandType() {
	return this->commandType;
}

void ICommand::execute() {
	IETThread::sleep(10);

	//IETThread::sleep(Delay::PER_INSTRUCTION_DELAY)
}

ICommand::ICommand(int processID, CommandType commandType) {
	this->processID = processID;
	this->commandType = commandType;
}