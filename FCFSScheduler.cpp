#include "FCFSScheduler.h"

#include <iostream>
#include <thread>
#include <chrono>

FCFSScheduler::FCFSScheduler() {}

FCFSScheduler::FCFSScheduler(int cores) : numCores(cores), schedulerRun(true) {
    init();
}

void FCFSScheduler::init() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    cpuWorkers.clear();
    currentIndex.clear();
    while (!globalQueue.empty()) globalQueue.pop();  // clear global queue

    for (int i = 0; i < numCores; ++i) {
        cpuWorkers.push_back(std::make_shared<SchedulerWorker>(i));
        currentIndex.push_back(0);
    }
}

void FCFSScheduler::run() {
    schedulerRun = true;
    schedulerThread = std::thread([this]() {
        while (schedulerRun) {
            execute();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // simulate CPU tick
        }
    });
}

void FCFSScheduler::stop() {
    schedulerRun = false;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void FCFSScheduler::execute() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    for (int core = 0; core < numCores; ++core) {
        auto& worker = cpuWorkers[core];

        if (!worker->isBusy() && !globalQueue.empty()) {
            auto process = globalQueue.front();
            globalQueue.pop();

            /*std::cout << "[FCFS] Assigning process " << process->getName()
                << " to core " << core << std::endl;*/

            worker->assignProcess(process);
            worker->start();
        }
    }
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core) {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    globalQueue.push(process);  // ignore core and just queue
}

void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core) {
    addProcess(process, core);  // still uses global queue
}

std::shared_ptr<Process> FCFSScheduler::getProcess(int core) const {
    if (core >= 0 && core < numCores && cpuWorkers[core]->isBusy()) {
        return cpuWorkers[core]->getProcess();
    }
    return nullptr;
}

const std::string& FCFSScheduler::getProcessFromQueue(int index) const {
    static std::string empty = "";
    //std::lock_guard<std::mutex> lock(schedulerMutex);
    if (!globalQueue.empty()) {
        return globalQueue.front()->getName();  // Only returns front of global queue
    }
    return empty;
}

void FCFSScheduler::printCores() {
    for (int i = 0; i < numCores; ++i) {
        std::cout << "Core " << i << ": ";
        if (cpuWorkers[i]->isBusy()) {
            std::cout << cpuWorkers[i]->getProcess()->getName();
        } else {
            std::cout << "[Idle]";
        }
        std::cout << std::endl;
    }
}

void FCFSScheduler::printProcessQueues() {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    std::cout << "Global Queue: ";
    std::queue<std::shared_ptr<Process>> tempQueue = globalQueue;
    while (!tempQueue.empty()) {
        std::cout << tempQueue.front()->getName() << " ";
        tempQueue.pop();
    }
    std::cout << std::endl;
}

bool FCFSScheduler::allProcessesFinished() {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    if (!globalQueue.empty()) return false;
    for (int i = 0; i < numCores; ++i) {
        if (cpuWorkers[i]->isBusy()) {
            return false;
        }
    }
    return true;
}
