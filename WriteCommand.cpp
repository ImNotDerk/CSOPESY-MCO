#include "WriteCommand.h"

// implement logic to write a uint16_t value to the specified memory address
WriteCommand::WriteCommand(const String& address, uint16_t value, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
    : ICommand(processID, WRITE), address(address), value(value), symbolTable(symbolTable)
{
}

std::shared_ptr<ICommand> WriteCommand::clone() const {
    return std::make_shared<WriteCommand>(*this);
}

void WriteCommand::execute()
{
    ICommand::execute();
    reWrite();
}

void WriteCommand::reWrite() 
{
    // add write logic here
}

String WriteCommand::getOutput() const
{
    return "Write: Giving value " + std::to_string(value) + " to address " + address;
}