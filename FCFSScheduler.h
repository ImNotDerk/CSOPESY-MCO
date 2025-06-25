#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <string>

#include "Process.h"
#include "AScheduler.h"
#include "SchedulerWorker.h"

// First-Come, First-Served Scheduler
class FCFSScheduler : public AScheduler {
public:
    using CPUWorkers = std::vector<std::shared_ptr<SchedulerWorker>>;

    FCFSScheduler();
    explicit FCFSScheduler(int cores);
    ~FCFSScheduler() override = default;

    // Lifecycle
    void init() override;
    void run() override;
    void stop() override;
    void execute() override;

    // Process management
    void assignProcess(std::shared_ptr<Process> process) override;
    void addProcess(std::shared_ptr<Process> process, int core) override;
    void assignCore(std::shared_ptr<Process> process, int core) override;

    // Core checks
    int checkCores() override;
    int checkCoreQueue() override;

    // Utility
    std::shared_ptr<Process> getProcess(int core) const;
    const std::string& getProcessFromQueue(int index) const override;

    void printCores() override;
    void printProcessQueues();

    // Scheduler state
    bool allProcessesFinished();

private:
    std::string name = "FCFS";
    int numCores = 1;

    std::mutex schedulerMutex;

    std::thread schedulerThread;
    std::atomic<bool> schedulerRun = false;


    CPUWorkers cpuWorkers;
    std::vector<std::queue<std::shared_ptr<Process>>> processQueues;
    std::vector<int> currentIndex;
};
