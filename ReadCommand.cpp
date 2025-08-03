#include "ReadCommand.h"

// implement logic to check whether memory block is initialized, if not read returns 0
ReadCommand::ReadCommand(const String& varName, const String& address) : ICommand(processID, READ)
{
    this->varName = varName;
    this->value = 0;
}

ReadCommand::ReadCommand(const String& varName) : ICommand(processID, READ)
{
    this->varName = varName;
    this->value = 0;
}

std::shared_ptr<ICommand> ReadCommand::clone() const {
    return std::make_shared<ReadCommand>(*this);
}

void ReadCommand::execute()
{
    ICommand::execute(); 
    getValue();
}

uint16_t ReadCommand::getValue() const
{
    return value;
}

String ReadCommand::getOutput() const
{
    return "Read: " + varName + " = " + std::to_string(value);
}