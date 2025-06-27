#include "DeclareCommand.h"

// Constructor for DECLARE: takes variable name and initial value
DeclareCommand::DeclareCommand(const String& varName, uint16_t value)
    : ICommand(processID, CommandType::DECLARE), var(varName), value(value) {
    // IMPORTANT: Variables are only stored temporarily. To make them "appear" or be associated in the process,  
    // the actual declaration must occur in the execute command.  
}

void DeclareCommand::execute()
{
	ICommand::execute();
	declareVariable(getVariableName());
}

void DeclareCommand::declareVariable(const String& varName) {
	this->var = varName;
	this->value = static_cast<uint16_t>(rand() % 65536); // clamp range of uint16_t from 0 to 65535
}

String DeclareCommand::getVariableName() {
	return this->var;
}

uint16_t DeclareCommand::getValue() {
	return this->value;
}

String DeclareCommand::getOutput() const {  
	String msg = this->var + " declared with the value: " + std::to_string(this->value) + "!";
    return msg;  
}
