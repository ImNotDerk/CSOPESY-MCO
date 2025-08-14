#include "ICommand.h"

ICommand::CommandType ICommand::getCommandType() {
	return this->commandType;
}

void ICommand::execute() {

	IETThread::sleep(ConfigReader::getInstance()->getDelays());
}

ICommand::ICommand(int processID, CommandType commandType) {
	this->processID = processID;
	this->commandType = commandType;
	this->pageNumber = -1; // default value, will be set later if needed
}
