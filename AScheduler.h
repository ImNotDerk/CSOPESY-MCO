#pragma once
#include "IETThread.h"
#include "Process.h"

#include <sstream>
#include <iostream>

typedef std::string String;

static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";

class AScheduler : public IETThread
{
public:
	enum SchedulingAlgorithm
	{
		FCFS
	};

	AScheduler();
	AScheduler(SchedulingAlgorithm schedulingAglo, int pid, String processName);
	std::shared_ptr<Process> findProcess();
	~AScheduler() = default;

	std::shared_ptr<Process> findProcess(String processName);
	void run() override;
	void stop();

	virtual int checkCores() = 0;
	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void addProcess(std::shared_ptr<Process> process, int core) = 0;
	virtual void assignCore(std::shared_ptr<Process> process, int core) = 0;
	virtual int checkCoreQueue() = 0;
	virtual String getProcessfromQueue(int index) const = 0;
	virtual void printCore() = 0;
	virtual void printProcessQueue() = 0;

	virtual void schedulerStart() = 0;
	virtual void schedulerStop() = 0;

	virtual String getName() const = 0;

	friend class GlobalScheduler;
	
private:
	SchedulingAlgorithm schedulingAlgo;
	int pid;
	String processName;
	bool running = true;

protected:
	std::vector<std::shared_ptr<Process>> processes;
};

