#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "MemoryManager.h"
#include <iostream>
#include <chrono>
#include <thread>

// Initialize the static instance pointer
GlobalScheduler* GlobalScheduler::instance = nullptr;

GlobalScheduler::GlobalScheduler() {
    // Default: use FCFS scheduler
    this->scheduler = std::make_shared<FCFSScheduler>();
    // Initialize memory manager with total memory size, frame size, and process size
    memoryManager = std::make_unique<MemoryManager>(16384, 16, 4096); // 16384 bytes total memory, 16 bytes per frame, 4096 bytes per process
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
    else if (algoName == "rr") {
        this->scheduler = std::make_shared<RRScheduler>(coreCount, ConfigReader::getInstance()->getQuantum());
    }
}

// Assign dummy processes to the scheduler and start it
void GlobalScheduler::schedulerStart() {
    scheduler_start = true;
    int lastSpawnTick = 0;
    int batchFreq = ConfigReader::getInstance()->getBatchProcessFreq();

    schedulerThread = std::thread([this, batchFreq, lastSpawnTick]() mutable {
        while (scheduler_start) {
            int currentTick = CPUTick::getInstance()->getTicks();

            if ((currentTick - lastSpawnTick) >= batchFreq) {
                int i = getProcessCount();
                i++;
                std::string name = "process_" + std::to_string(i);
                auto process = std::make_shared<Process>(i, name);

                // Attempt to allocate memory for the process
                if (memoryManager->allocateMemory(i)) {
                    ConsoleManager::getInstance()->createBaseScreen(process, false);
                    processList.push_back(process);
                    this->scheduler->addProcess(process, -1);
                }
                else {
                    std::cout << "Memory allocation failed for process: " << name << std::endl;
                }

                lastSpawnTick = currentTick; // Update last spawn tick
            }

            // Periodically save memory snapshots
            if (currentTick % 10 == 0) {  // Save snapshot every 10 ticks
                memoryManager->saveMemorySnapshot(currentTick);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Keep thread responsive
        }
        });
}

void GlobalScheduler::schedulerStop() {
    scheduler_start = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

// Add a new process
void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
    processList.push_back(process);
    this->scheduler->addProcess(process, -1);
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