#include "Process.h"
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

