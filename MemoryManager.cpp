#include "MemoryManager.h"  // Include the header file for MemoryManager

MemoryManager* MemoryManager::sharedInstance = nullptr;

void MemoryManager::initialize(int maxOverallMemory, int memFrameSize, int memPerProc) {
    if(sharedInstance == nullptr) {
        sharedInstance = new MemoryManager(maxOverallMemory, memFrameSize, memPerProc);
	}
}

void MemoryManager::destroy() {
    if (sharedInstance != nullptr) {
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

MemoryManager* MemoryManager::getInstance() {
    return sharedInstance;
}

bool MemoryManager::allocateMemory(int processID) {
    // Check if the process is already allocated
    if (isAllocated(processID)) {
        return false; // Process already has a frame
    }

    // Find the first free frame and allocate the process
    for (int& frame : memory) {
        if (frame == 0) {
            frame = processID;
            return true;
        }
    }
    return false; // No free frame available
}

bool MemoryManager::deallocateMemory(int processID) {
    bool deallocated = false;
    for (int& frame : memory) {
        if (frame == processID) {
            frame = 0;
            deallocated = true;
        }
    }
    return deallocated;
}

bool MemoryManager::isAllocated(int processID) const {
    return std::find(memory.begin(), memory.end(), processID) != memory.end();
}

int MemoryManager::getExternalFragmentation() const {
    int freeSpace = std::count(memory.begin(), memory.end(), 0);
    return freeSpace * memPerProc;
}

void MemoryManager::saveMemorySnapshot(int cycle) const {
    std::string folder = "snapshots";
    std::string filename = folder + "/memory_stamp_" + std::to_string(cycle) + ".txt";
    std::ofstream file(filename);

    if (!file.is_open()) {
        return;
    }

    // Get current time as string using std::chrono and localtime_s
    auto now = std::chrono::system_clock::now();
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time_t_now);

    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm);

    file << "Timestamp: (" << timeStr << ") " << "\n";
    file << "Processes in memory: " << getAllocatedProcessCount() << "\n";
    file << "Total external fragmentation in KB: " << getExternalFragmentation() << "\n";
    file << "Memory Snapshot:\n";

    file << "---end--- = " << maxOverallMemory << "\n";
    int addr = maxOverallMemory;
    for (int i = memory.size() - 1; i >= 0; --i) {
        if (memory[i] != 0) {
            file << "\n";
            file << addr << "\n";
            file << "P" << memory[i] << "\n";
            addr -= memPerProc;
            file << addr << "\n";
        }
        else {
            addr -= memPerProc; // still decrease addr even if not printing
        }
    }

    file << "\n---start--- = 0\n";
}
int MemoryManager::getAllocatedProcessCount() const {
    std::unordered_set<int> allocatedProcesses;
    for (int i : memory) {
        if (i != 0) {
            allocatedProcesses.insert(i);
        }
    }
    return allocatedProcesses.size();
}

void MemoryManager::setMaxOverallMemory(int maxOverallMemory) {
	this->maxOverallMemory = maxOverallMemory;
}

void MemoryManager::setMemPerFrame(int memFrameSize) {
    this->memPerFrame = memFrameSize;
}

void MemoryManager::setMemPerProc(int memPerProc) {
    this->memPerProc = memPerProc;
}

MemoryManager::MemoryManager(int maxOverallMemory, int memPerFrame, int memPerProc)
    : maxOverallMemory(maxOverallMemory), memPerFrame(memPerFrame), memPerProc(memPerProc) {
	this->numFrames = maxOverallMemory / memPerProc; // Calculate number of frames
    this->memory.resize(numFrames, 0);
}

