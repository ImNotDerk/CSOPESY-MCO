#include "SubtractCommand.h"

void SubtractCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior

	uint16_t result = performSubtraction();

	// NOTE: print for the mean time
	std::cout << "Subtraction Result: " << result << std::endl;
}

uint16_t SubtractCommand::performSubtraction()
{
	// add val2 and 3 and store in val1
	this->var1 = this->var2 + this->var3;
	return this->var1;
}