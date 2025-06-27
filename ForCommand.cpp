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

void ForCommand::performWithLogging(Process* process, int coreId, int currentDepth)
{
	const int MAX_DEPTH = 3;
	if (currentDepth > MAX_DEPTH) return;

	std::string indentation(currentDepth * 2, ' ');

	for (int i = 0; i < this->repeats; ++i)
	{
		for (ICommand* instruction : this->instructions)
		{
			if (!instruction) continue;

			if (auto* nested = dynamic_cast<ForCommand*>(instruction)) {
				nested->performWithLogging(process, coreId, currentDepth + 1);
			}
			else {
				instruction->execute();
				String output = instruction->getOutput();
				output = indentation + output;

				process->logInstruction(coreId, output);

				if (output.find("Debug:: Exit FOR") == String::npos) {
					process->incrementCommandCounter();
				}
			}
		}
	}
}