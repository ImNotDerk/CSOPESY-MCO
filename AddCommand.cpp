#include "AddCommand.h"

AddCommand::AddCommand(uint16_t operand1, uint16_t operand2) : ICommand(processID, CommandType::ADD)
{
	this->var1 = 0;
	this->var2 = operand1;
	this->var3 = operand2;
}

void AddCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior
	performAddition();
}

std::shared_ptr<ICommand> AddCommand::clone() const {
	return std::make_shared<AddCommand>(*this);
}

void AddCommand::performAddition()
{
	this->var1 = this->var2 + this->var3;
}

uint16_t AddCommand::getResult() const
{
	return this->var1;
}

String AddCommand::getOutput() const
{
	return "Result of addition: " + std::to_string(this->var1);
}