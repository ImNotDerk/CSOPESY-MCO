#pragma once
#include "ICommand.h"
class AddCommand : public ICommand
{
public:
	AddCommand(uint16_t var2, uint16_t var3);
	void performAddition();
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;
	uint16_t getResult() const;
	String getOutput() const override;

private:
	uint16_t var1; // result
	uint16_t var2; // value 1
	uint16_t var3; // value 2
};

