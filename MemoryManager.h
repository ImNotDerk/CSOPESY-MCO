#pragma once
#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <algorithm>  // For std::all_of
#include <string>   // For std::to_string
#include <ctime>
#include <chrono>

class MemoryManager {
public:
	// Default Constructor and Destructor
    MemoryManager(int maxOverallMemory, int memFrameSize, int memPerProc);
    ~MemoryManager() = default;

    // Singleton access
    static MemoryManager* getInstance();
    static void initialize(int maxOverallMemory, int memFrameSize, int memPerProc);
    static void destroy();

    // Function to allocate memory for a process
    bool allocateMemory(int processID);

    // Function to deallocate memory for a process
    bool deallocateMemory(int processID);

    bool isAllocated(int processID) const;

    // Function to get the external fragmentation in KB
    int getExternalFragmentation() const;

    // Function to save a snapshot of memory to a file
    void saveMemorySnapshot(int cycle) const;

    void setMaxOverallMemory(int maxOverallMemory);
    void setMemPerFrame(int memPerFrame);
    void setMemPerProc(int memPerProc);

private:
    // Disallow copying
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager const&) {}

    // Singleton instance
    static MemoryManager* sharedInstance;

    int maxOverallMemory = 0;     // Total memory size (in bytes)
    int memPerFrame = 0;          // Size of each memory frame (in bytes)
	int numFrames = 0;            // Number of memory frames (THIS IS JUST FOR HOMEWORK 10. Should be removed in the future)
    int memPerProc = 0;           // Size of each process (in bytes)
    std::vector<int> memory;    // Vector representing the memory (0 = free, >0 = process ID)

    // Helper function to get the count of allocated processes
    int getAllocatedProcessCount() const;
};