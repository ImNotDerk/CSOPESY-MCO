#pragma once
#include "ICommand.h"
class AddCommand : public ICommand
{
public:
	AddCommand(int processID, uint16_t var1, uint16_t var2, uint16_t var3) : ICommand(processID, CommandType::ADD)
	{
		this->var1 = var1;
		this->var2 = var2;
		this->var3 = var3;
	}

	uint16_t performAddition();
	void execute() override;

private:
	uint16_t var1; // result
	uint16_t var2; // value 1
	uint16_t var3; // value 2
};

