#include "SchedulerWorker.h"

SchedulerWorker::SchedulerWorker(int coreId)
    : coreId(coreId), running(false), busy(false), currentProcess(nullptr) {
}

SchedulerWorker::~SchedulerWorker() {
    stop();  // Ensure thread joins on destruction
}

void SchedulerWorker::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&SchedulerWorker::run, this);
    }
}

void SchedulerWorker::stop() {
    running = false;
    cv.notify_all();  // Wake thread if waiting
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void SchedulerWorker::assignProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    currentProcess = process;
    busy = true;
    cv.notify_one();  // Wake run() thread
}

bool SchedulerWorker::isBusy() const {
    return busy;
}

std::shared_ptr<Process> SchedulerWorker::getProcess() const {
    std::lock_guard<std::mutex> lock(mtx);
    return currentProcess;
}

void SchedulerWorker::run() {
    int delays_per_exec = ConfigReader::getInstance()->getDelays();
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);

        int idleStart = CPUTick::getInstance()->getTicks();
        cv.wait(lock, [&]() { return currentProcess != nullptr || !running; });
        int idleEnd = CPUTick::getInstance()->getTicks();
        CPUTick::getInstance()->addIdleCpuTicks(idleEnd - idleStart);

        cv.wait(lock, [&]() { return currentProcess != nullptr || !running; });

        if (!running) break;

        auto process = currentProcess;  // Copy safely while holding lock
        int startTick = CPUTick::getInstance()->getTicks();
        int lastExecutedTick = startTick;
        lock.unlock();

        while (process && !process->isFinished()) {
            int currentTick = CPUTick::getInstance()->getTicks();
            auto page = process->getPageForInstruction(process->getCommandCounter());

            if (!page->isPageValid()) {
                MemoryManager::getInstance()->handlePageFault(process->getPID(), page);
            }

            if (currentTick > lastExecutedTick) {
                process->executeCurrentCommand(coreId);

                CPUTick::getInstance()->addActiveCpuTicks(1); // Only once per tick
                lastExecutedTick = currentTick;
            }
        }


        process->setState(Process::FINISHED);

        lock.lock();
        currentProcess = nullptr;
        busy = false;
        lock.unlock();
    }
}