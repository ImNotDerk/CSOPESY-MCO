#include "BaseScreen.h"

BaseScreen::BaseScreen(shared_ptr<Process> process, String processName) {
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
		cout << "" << endl;
		cout << "Process: " << this->attachedProcess->getName() << endl;
		cout << "ID: " << this->attachedProcess->getPID() << endl;
		cout << "" << endl;
		cout << "Current instruction line: " << this->attachedProcess->getCommandCounter() << endl;
		cout << "Lines of code: " << this->attachedProcess->getLinesOfCode() << endl;
		cout << "" << endl;
	}
	else
	{
		cout << "" << endl;
		cout << "Process: " << this->attachedProcess->getName() << endl;
		cout << "ID: " << this->attachedProcess->getPID() << endl;
		cout << "" << endl;
		cout << "Finished!" << endl;
		cout << "" << endl;
	}
}