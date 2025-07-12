#pragma once

#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>

class MemoryManager {
public:
    // Constructor to initialize memory with total size, frame size, and process size
    MemoryManager(int totalMemory, int frameSize, int procSize);

    // Function to allocate memory for a process
    bool allocateMemory(int processID);

    // Function to get the external fragmentation in KB
    int getExternalFragmentation() const;

    // Function to save a snapshot of memory to a file
    void saveMemorySnapshot(int cycle) const;

private:
    int totalMemory;        // Total memory size (in bytes)
    int frameSize;          // Size of each memory frame (in bytes)
    int procSize;           // Size of each process (in bytes)
    std::vector<int> memory; // Vector representing the memory (0 = free, >0 = process ID)

    // Helper function to get the count of allocated processes
    int getAllocatedProcessCount() const;
};