#include "PrintCommand.h"
#include "IETThread.h"

PrintCommand::PrintCommand(int processID, String processName, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
	: ICommand(processID, PRINT), processName(processName), symbolTable(symbolTable) {
}

std::shared_ptr<ICommand> PrintCommand::clone() const {
	return std::make_shared<PrintCommand>(*this);
}

void PrintCommand::execute()
{
	ICommand::execute();
	std::cout << toPrint << std::endl;
}

void PrintCommand::setString()
{
	if (!symbolTable->empty())
	{
		bool msgOrNone = rand() % 2; // 0 for no message, 1 for message

		if (msgOrNone) {
			int randomIndex = rand() % symbolTable->size();
			auto it = symbolTable->begin();
			std::advance(it, randomIndex);

			uint16_t varValue = it->second;
			toPrint = "Print value from: " + std::to_string(varValue);
		}
		else {
			toPrint = "Hello world from " + processName;
		}
	}
	else
	{
		toPrint = "Hello world from " + processName;
	}
}

String PrintCommand::getOutput() const
{

	return toPrint;
}