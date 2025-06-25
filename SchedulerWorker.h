#pragma once  
#include "IETThread.h"  
#include "Process.h"  
#include <thread>  
#include <mutex>  
#include <queue>  

class SchedulerWorker : public IETThread  
{  
public:  
    SchedulerWorker() = default;  
    void update(bool isRunning);  
    void run() override;  
    void isOccupied();  
    void addProcess(std::shared_ptr<Process> process);  
    bool isAvailable() const;  
    bool doesProcessExist() const;  
    int getNumCores() const;  

private:  
    bool isRunning = true;  
    bool available = true;  
    int numCores;  
    std::shared_ptr<Process> currentProcess; 
    std::queue<std::shared_ptr<Process>> processQueue; 
};
