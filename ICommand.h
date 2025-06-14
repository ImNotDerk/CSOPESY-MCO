#pragma once
#include "IETThread.h"
#include <string>

typedef std::string String;
// icommand
class ICommand
{
public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(int processID, CommandType commandtype);
	CommandType	getCommandType();
	virtual void execute();

protected:
	int processID;
	CommandType commandType;
};

inline ICommand::CommandType ICommand::getCommandType() {
	return this->commandType;
}

inline void ICommand::execute() {
	IETThread::sleep(10);
}

inline ICommand::ICommand(int processID, CommandType commandType) {
	this->processID = processID;
	this->commandType = commandType;