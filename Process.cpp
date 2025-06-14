#include "Process.h"
#include "CommandPrinter.h"
#include "ICommand.h"
#include <string>

typedef std::string String;

Process::Process(int pid, String name) {
	this->pid = pid;
	this->name = name;
	this->commandCounter = 0;
	this->currentState = Process::ProcessState::READY;
}

void Process::executeCurrentCommand() const {
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine() {
	this->commandCounter++;
}

bool Process::isFinished() const {
	return this->commandCounter >= this->commandList.size();
}

void Process::addCommand(ICommand::CommandType commandType) {
	String printAdd = "Command added!";
	const shared_ptr<ICommand> command = make_shared<CommandPrinter>(this->pid, printAdd);

	if (command == nullptr)
	{
		cerr << "Failed to create command: No command declared [NULL detected]." << endl;
		return;
	}
	this->commandList.push_back(command);
}

int Process::incrementCommandCounter() {
	return this->commandCounter++;
}

int Process::getRemainingTime() const {
	return this->commandCounter; //commandCounter for now...
}

int Process::getCommandCounter() const {
	return this->commandCounter;
}

int Process::getPID() const{
	return this->pid;
}

int Process::getCPUCoreID() const {  
    return this->cpuCoreID;  
}

Process::ProcessState Process::getState() const {  
    return this->currentState;  
}

int Process::getLinesOfCode() const {
	return this->commandList.size();
}

