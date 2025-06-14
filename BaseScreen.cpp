#include "BaseScreen.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) {
	process = make_shared<Process>(processName);
	this->attachedProcess = process;
}

void BaseScreen::onEnabled() {
	system("cls");
	this->printProcessInfo();
}

void BaseScreen::printProcessInfo() const
{
	if (!this->attachedProcess->isFinished())
	{
		std::cout << "" << std::endl;
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << this->attachedProcess->getLinesOfCode() << std::endl;
		std::cout << "" << std::endl;
	}
	else
	{
		std::cout << "" << std::endl;
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "Finished!" << std::endl;
		std::cout << "" << std::endl;
	}
}