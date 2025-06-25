#include "SchedulerWorker.h"
//#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning) {
	this->isRunning = isRunning;
}

void SchedulerWorker::run() {
	while (isRunning) {
		//GlobalScheduler::getInstance()->tick();
		//for memory allocation
	}
}

void SchedulerWorker::addProcess(std::shared_ptr<Process> process) {
	this->processQueue.push(process);
	this->currentProcess = processQueue.front();
}

void SchedulerWorker::isOccupied() {

	this->available = false;
}

bool SchedulerWorker::isAvailable() const {
	return this->available;
}

bool SchedulerWorker::doesProcessExist() const {
	if (this->currentProcess != nullptr) {
		return true;
	}
	else {
		return false;
	}
}

int SchedulerWorker::getNumCores() const {
	return this->numCores; 
}

