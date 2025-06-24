#pragma once
#include "ICommand.h"
class DeclareCommand : public ICommand
{
public: 
	DeclareCommand(int processID, String& varName, uint16_t value) : ICommand(processID, CommandType::DECLARE)
	{
		this->varName = varName;
		this->value = value;
		//IMPORTANT: Variables are only stored temporarily. To make them "appear" or be associated in the process, 
		//the actual declaration must occur in the execute command.
	}

	void execute() override;
	
	//TODO: The actual declaration --> symbol table association + placement in memory.
	//NOTE: Consider using std::move or move-based operation for transferring the data without copying
	/*void performDeclaration();*/

private:
	String varName; // Variable name
	uint16_t value; // Initial value
};

