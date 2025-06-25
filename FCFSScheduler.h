#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <algorithm>
#include "AScheduler.h"

typedef std::vector<std::shared_ptr<SchedulerWorker>> CPUWorkers;

class FCFSScheduler: public AScheduler
{
public:
	FCFSScheduler(int cores);

	void run() override;
	void init() override;
	void execute()override;

	void addProcess(std::shared_ptr<Process> process, int core) override; // add process to core worker
	void assignCore(std::shared_ptr<Process>, int core) // assign core to process
};

