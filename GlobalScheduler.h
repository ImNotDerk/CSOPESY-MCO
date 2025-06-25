#pragma once

#include <vector>
#include <memory>
#include "AScheduler.h"
#include "Process.h"

// Singleton class to manage the selected scheduler and process list across the application.
class GlobalScheduler {
public:
    // Singleton access
    static GlobalScheduler* getInstance();
    static void initialize();
    static void destroy();

    // Scheduler control
    void selectScheduler(const std::string& algoName);
    std::shared_ptr<AScheduler> getScheduler();
    void tick();  // Optional: step-based simulation or update

    // Process management
    void addProcess(std::shared_ptr<Process> process);
    int getProcessCount() const;
    std::shared_ptr<Process>& getProcess(int index);
    std::shared_ptr<Process> getMostRecentProcess();
    void schedulerStart(); // this is where dummy processes are generated
    void schedulerStop();

    // Core configuration
    void setCoreCount(int cores);
    int getCoreCount() const;

private:
    // Constructor and destructor
    GlobalScheduler();
    ~GlobalScheduler();

    // Disallow copying
    GlobalScheduler(const GlobalScheduler&) = delete;
    GlobalScheduler& operator=(const GlobalScheduler&) = delete;

    // Singleton instance
    static GlobalScheduler* instance;

    // Current scheduler
    std::shared_ptr<AScheduler> scheduler;

    // List of all submitted processes
    std::vector<std::shared_ptr<Process>> processList;

    // Core configuration
    int coreCount = 1;
};
