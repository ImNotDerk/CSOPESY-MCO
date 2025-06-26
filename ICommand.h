#pragma once
#include "IETThread.h"
#include <string>

typedef std::string String;

class ICommand
{
public:
	enum CommandType
	{
		PRINT,
		DECLARE,
		ADD,
		SUBTRACT,
		SLEEP,
		FOR
	};
	
	ICommand(int processID, CommandType commandtype);
	CommandType	getCommandType();
	virtual void execute();
	virtual String getOutput() const { return ""; }  // default empty

protected:
	int processID;
	CommandType commandType;
};