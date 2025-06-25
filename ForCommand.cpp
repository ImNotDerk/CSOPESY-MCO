#include "ForCommand.h"

void ForCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior

	performForCommand();
}

void ForCommand::performForCommand()
{
    for (int i = 0; i < this->repeats; i++)
    {
        for (ICommand* instruction : instructions)
        {
            if (instruction != nullptr)
            {
                instruction->execute();
            }
        }
    }
}