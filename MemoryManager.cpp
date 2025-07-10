#include "MemoryManager.h"  // Include the header file for MemoryManager
#include <algorithm>  // For std::all_of
#include <fstream>    // For file output
#include <unordered_set> // For getAllocatedProcessCount

MemoryManager::MemoryManager(int totalMemory, int frameSize, int procSize)
    : totalMemory(totalMemory), frameSize(frameSize), procSize(procSize) {
    memory.resize(totalMemory / frameSize, 0);  // 0 means free
}

bool MemoryManager::allocateMemory(int processID) {
    int numFrames = procSize / frameSize;
    for (size_t i = 0; i <= memory.size() - numFrames; ++i) {
        // Check if there are enough consecutive free frames to allocate the process
        if (std::all_of(memory.begin() + i, memory.begin() + i + numFrames, [](int x) { return x == 0; })) {
            std::fill(memory.begin() + i, memory.begin() + i + numFrames, processID);  // Mark as allocated
            return true;
        }
    }
    return false;  // No space available
}

int MemoryManager::getExternalFragmentation() const {
    // Count the number of free frames and convert to KB
    int freeSpace = std::count(memory.begin(), memory.end(), 0);
    return freeSpace * frameSize / 1024;  // Convert to KB
}

void MemoryManager::saveMemorySnapshot(int cycle) const {
    std::ofstream file("memory_stamp_" + std::to_string(cycle) + ".txt");
    file << "Timestamp: " << cycle << "\n";
    file << "Processes in memory: " << getAllocatedProcessCount() << "\n";
    file << "External Fragmentation: " << getExternalFragmentation() << " KB\n";
    file << "Memory Snapshot:\n";

    // Print memory snapshot (each process in memory or free space)
    for (size_t i = 0; i < memory.size(); ++i) {
        if (i % 10 == 0) file << "\n";  // Formatting for readability
        file << memory[i] << " ";  // 0 = free, process ID > 0
    }
    file << "\n";
}

int MemoryManager::getAllocatedProcessCount() const {
    std::unordered_set<int> allocatedProcesses;
    // Iterate through memory and add process IDs to set
    for (int i : memory) {
        if (i != 0) allocatedProcesses.insert(i);
    }
    return allocatedProcesses.size();  // Return the number of unique processes
}