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
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return currentProcess != nullptr || !running; });

        if (!running) break;

        auto process = currentProcess;  // Copy safely while holding lock
        lock.unlock();

        std::cout << "[Core " << coreId << "] Running process: "
            << process->getName() << std::endl;

        while (process && !process->isFinished()) {
            process->executeCurrentCommand();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "[Core " << coreId << "] Finished process." << std::endl;

        // Reset state
        lock.lock();
        currentProcess = nullptr;
        busy = false;
        lock.unlock();
    }
}
