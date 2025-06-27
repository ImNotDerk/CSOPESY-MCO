#include "ForCommand.h"

void ForCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior
	performForCommand();
}

void ForCommand::performForCommand()
{
	// Loop for the number of repeats
	for (int i = 0; i < this->repeats; i++)
	{
		// Execute each instruction in the instructions list
		for (ICommand* instruction : instructions)
		{
			if (instruction != nullptr)
			{
				instruction->execute();
			}
		}
	}
}