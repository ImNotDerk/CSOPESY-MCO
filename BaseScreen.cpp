#include "BaseScreen.h"
#include "ConsoleManager.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) {
	this->attachedProcess = process;
	this->userInput = "";
	this->commandInput = "";
	this->name = processName;
}

void BaseScreen::onEnabled() {
	system("cls");
	this->printProcessInfo();
}

void BaseScreen::display() {
	if (this->commandInput.empty())
	{
		onEnabled();
	}
	else if (this->commandInput == "exit")
	{
		ConsoleManager::getInstance()->returnToPreviousConsole();
		this->commandInput.clear();
	}
	else if (this->commandInput == "clear")
	{
		system("cls");
		this->commandInput.clear();
	}
	else if (commandInput == "process-smi")
	{
		this->printProcessInfo();
		this->commandInput.clear();
	}
	else
	{
		this->commandInput.clear();
	}
}

void BaseScreen::process() {
	if (this->refreshed == false)
	{
		this->refreshed = true;
	}

	std::cout << "root:\\\\> ";
	getline(std::cin, this->userInput); // get user input

	if (this->userInput == "exit")
	{
		this->commandInput = this->userInput;
	}
	else if (this->userInput == "clear")
	{
		this->commandInput = userInput;
	}
	else if (this->userInput == "process-smi")
	{
		this->commandInput = this->userInput;
	}
	else
	{
		std::cout << "Unknown command: " << this->userInput << std::endl;
		this->commandInput = this->userInput;
	}
}

String BaseScreen::getName() const
{
	return this->attachedProcess->getName();
}

std::shared_ptr<Process> BaseScreen::getProcess() const
{
	return this->attachedProcess;
}

// Process-smi:
// TODO: Need to somehow store the logs of when specific commands were executed
/* sample:
* 
	Process name: screen_01
	ID: 1 
	Logs:
	(6/25/2025 11:05:30AM) Core 0: "Hello world from screen_01"
	(6/25/2025 11:05:35AM) Core 0: "Hello world from screen_01"
	(6/25/2025 11:07:20AM) Core 0: "Hello world from screen_01"

	Current instruction line: 10
	Lines of code: 1500
*/

void BaseScreen::printProcessInfo() const
{
	if (!this->attachedProcess->isFinished())
	{
		std::cout << "" << std::endl;
		this->attachedProcess->getCommandCounter();
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << this->attachedProcess->getLinesOfCode() << std::endl;
		// this is to check if commands are being stored
		// this->attachedProcess->printCommands();
		std::cout << "" << std::endl;
	}
	else
	{
		std::cout << "" << std::endl;
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
		// this is to check if commands are being stored
		// this->attachedProcess->printCommands();
		std::cout << "" << std::endl;
		std::cout << "Finished!" << std::endl;
		std::cout << "" << std::endl;
	}
}