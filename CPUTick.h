#pragma once

#include <thread>
#include <atomic>
#include <chrono>

class CPUTick {
public:
    static CPUTick* getInstance();
    static void destroy();  // Clean up memory

    void resetTicks();
    void addTick(int ticks);
    int getTicks() const;

	void addIdleCpuTicks(int ticks);
	void addActiveCpuTicks(int ticks);
	int getIdleCpuTicks() const;
	int getActiveCpuTicks() const;

    void startAutoTick(int intervalMs = 100);  // Start ticking in background
    void stopAutoTick();                       // Stop background ticking

    // Delete copy and assignment
    CPUTick(const CPUTick&) = delete;
    CPUTick& operator=(const CPUTick&) = delete;

private:
    CPUTick();
    ~CPUTick();

    static CPUTick* sharedInstance;
    int nTicks;
    int idleCpuTicks;
	int activeCpuTicks;

    std::thread tickThread;
    std::atomic<bool> ticking;
};
