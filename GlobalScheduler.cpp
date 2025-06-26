#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
// #include "RoundRobinScheduler.h" // Optional

#include <iostream>

// Initialize the static instance pointer
GlobalScheduler* GlobalScheduler::instance = nullptr;

GlobalScheduler::GlobalScheduler() {
    // Default: use FCFS scheduler
    this->scheduler = std::make_shared<FCFSScheduler>();
}

GlobalScheduler::~GlobalScheduler() {
    schedulerStop();  // Cleanup running scheduler if any
}

void GlobalScheduler::initialize() {
    if (instance == nullptr) {
        instance = new GlobalScheduler();
    }
}

void GlobalScheduler::destroy() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

GlobalScheduler* GlobalScheduler::getInstance() {
    return instance;
}

// Select scheduling algorithm by name
void GlobalScheduler::selectScheduler(const std::string& algoName) {
    if (algoName == "fcfs") {
        this->scheduler = std::make_shared<FCFSScheduler>(coreCount);
    }
    // else if (algoName == "rr") {
    //     scheduler = std::make_shared<RoundRobinScheduler>(coreCount);
    // }


}

// Assign dummy processes to the scheduler and start it
void GlobalScheduler::schedulerStart() {

    scheduler_start = true;

    // Generate dummy processes
    schedulerThread = std::thread([this]() {
        while (scheduler_start) {
            int i = getProcessCount();
            i++;
            std::string name = "process_" + std::to_string(i);
            auto process = std::make_shared<Process>(i, name);

            ConsoleManager::getInstance()->createBaseScreen(process, false);

            processList.push_back(process);

            // Assign to scheduler (core will be handled internally)
            scheduler->addProcess(process, -1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        }
    });

}

void GlobalScheduler::schedulerStop() {
    scheduler_start = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void GlobalScheduler::tick() {
    // Optional: you can implement step-wise simulation or logging here
    std::cout << "[GlobalScheduler] Tick..." << std::endl;
}

// Add a new process
void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
    processList.push_back(process);
    if (scheduler) {
        scheduler->addProcess(process, -1);
    }
}

// Get total process count
int GlobalScheduler::getProcessCount() const {
    return static_cast<int>(processList.size());
}

// Get specific process
std::shared_ptr<Process>& GlobalScheduler::getProcess(int index) {
    return processList.at(index);
}

// Get most recently added process
std::shared_ptr<Process> GlobalScheduler::getMostRecentProcess() {
    if (!processList.empty()) {
        return processList.back();
    }
    return nullptr;
}

// Accessor for the current scheduler
std::shared_ptr<AScheduler> GlobalScheduler::getScheduler() {
    return scheduler;
}

// Core configuration
void GlobalScheduler::setCoreCount(int cores) {
    this->coreCount = cores;
}

int GlobalScheduler::getCoreCount() const {
    return coreCount;
}
