#include "FCFSScheduler.h"

#include <iostream>
#include <thread>
#include <chrono>

FCFSScheduler::FCFSScheduler() {
}

FCFSScheduler::FCFSScheduler(int cores) : numCores(cores), schedulerRun(true) {
    init();
}

void FCFSScheduler::init() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    cpuWorkers.clear();
    processQueues.clear();
    currentIndex.clear();

    for (int i = 0; i < numCores; ++i) {
        cpuWorkers.push_back(std::make_shared<SchedulerWorker>(i));
        processQueues.emplace_back();  // empty queue per core
        currentIndex.push_back(0);     // track where each core is in its queue
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


// Ended here, scheduler still not working

void FCFSScheduler::execute() {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    for (int core = 0; core < numCores; ++core) {
        auto& worker = cpuWorkers[core];
        auto& queue = processQueues[core];

        if (!worker->isBusy() && !queue.empty()) {
            auto process = queue.front();
            queue.pop();
            worker->assignProcess(process);
            worker->start();  // Start the thread running the process
        }
    }
}

void FCFSScheduler::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    int targetCore = checkCores();
    if (targetCore == -1) {
        // All cores are busy, push to shortest queue
        targetCore = checkCoreQueue();
    }

    processQueues[targetCore].push(process);
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process, int core) {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    if (core >= 0 && core < numCores) {
        processQueues[core].push(process);
    }
}

void FCFSScheduler::assignCore(std::shared_ptr<Process> process, int core) {
    addProcess(process, core);
}

int FCFSScheduler::checkCores() {
    for (int i = 0; i < numCores; ++i) {
        if (!cpuWorkers[i]->isBusy()) return i;
    }
    return -1;  // No free core
}

int FCFSScheduler::checkCoreQueue() {
    int minIndex = 0;
    int minSize = processQueues[0].size();

    for (int i = 1; i < numCores; ++i) {
        if (processQueues[i].size() < minSize) {
            minIndex = i;
            minSize = processQueues[i].size();
        }
    }

    return minIndex;
}

std::shared_ptr<Process> FCFSScheduler::getProcess(int core) const {
    if (core >= 0 && core < numCores && !processQueues[core].empty()) {
        return processQueues[core].front();
    }
    return nullptr;
}

const std::string& FCFSScheduler::getProcessFromQueue(int index) const {
    static std::string empty = "";
    if (index >= 0 && index < numCores && !processQueues[index].empty()) {
        return processQueues[index].front()->getName();
    }
    return empty;
}

void FCFSScheduler::printCores() {
    for (int i = 0; i < numCores; ++i) {
        std::cout << "Core " << i << ": ";
        if (cpuWorkers[i]->isBusy()) {
            std::cout << cpuWorkers[i]->getProcess()->getName();
        }
        else {
            std::cout << "[Idle]";
        }
        std::cout << std::endl;
    }
}

void FCFSScheduler::printProcessQueues() {
    for (int i = 0; i < numCores; ++i) {
        std::cout << "Queue " << i << ": ";
        auto tempQueue = processQueues[i];
        while (!tempQueue.empty()) {
            std::cout << tempQueue.front()->getName() << " ";
            tempQueue.pop();
        }
        std::cout << std::endl;
    }
}

bool FCFSScheduler::allProcessesFinished() {
    for (int i = 0; i < numCores; ++i) {
        if (cpuWorkers[i]->isBusy() || !processQueues[i].empty()) {
            return false;
        }
    }
    return true;
}
