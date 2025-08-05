#include "MemoryManager.h"  // Include the header file for MemoryManager
#include "GlobalScheduler.h"

MemoryManager* MemoryManager::sharedInstance = nullptr;

void MemoryManager::initialize(int maxOverallMemory, int memFrameSize, int minMemPerProc, int maxMemPerProc) 
{
    if(sharedInstance == nullptr) {
        sharedInstance = new MemoryManager(maxOverallMemory, memFrameSize, minMemPerProc, maxMemPerProc);
	}
}

void MemoryManager::destroy() 
{
    if (sharedInstance != nullptr) {
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

MemoryManager* MemoryManager::getInstance() 
{
    return sharedInstance;
}

//bool MemoryManager::loadPagesForProcess(std::shared_ptr<Process> process)
//{
//    int processID = process->getPID();
//    int pageSize = memPerFrame;
//
//
//    auto pageTable = process->getPageTable();
//    if (!pageTable) return false;
//
//    for (int pageNumber = 0; pageNumber < process->getPageTable()->size(); ++pageNumber) {
//        PageEntry& entry = (*pageTable)[pageNumber];
//
//        if (!entry.isPageValid()) {
//
//            // Find a free frame
//            auto it = std::find_if(memory.begin(), memory.end(), [](const FrameEntry& f) {
//                return !f.isFrameValid(); // find an invalid (free) frame
//                });
//
//            int frameNumber;
//
//            if (it == memory.end()) {   
//                // FIFO Page Replacement
//                if (fifoQueue.empty()) return false;
//
//                auto victim = fifoQueue.front(); // pair<processID, pageNumber>
//                fifoQueue.pop();
//
//                int victimPID = victim.first;
//                int victimPage = victim.second;
//
//                // Find the frame to evict
//                int evictFrame = -1;
//                for (int i = 0; i < memory.size(); ++i) {
//                    if (memory[i].getProcessID() == victimPID && memory[i].getPageNumber() == victimPage) {
//                        evictFrame = i;
//                        break;
//                    } 
//                }
//
//                if (evictFrame == -1) return false; // Should not happen
//
//                // Write evicted page to backing store
//                writeToBackingStore(victimPID, victimPage);
//                pagedOut++;
//
//                auto victimProcess = GlobalScheduler::getInstance()->getProcessByPID(victimPID);
//                if (victimProcess) {
//                    auto victimPageTable = victimProcess->getPageTable();
//                    if (victimPageTable && victimPage >= 0 && victimPage < victimPageTable->size()) {
//                        (*victimPageTable)[victimPage].invalidatePage();
//                    }
//                }
//
//                // Overwrite frame
//                memory[evictFrame] = FrameEntry{ processID, pageNumber, true };
//                frameNumber = evictFrame;
//            }
//            else {
//                // Free frame found
//                frameNumber = std::distance(memory.begin(), it);
//                memory[frameNumber] = FrameEntry{ processID, pageNumber, true };
//            }
//
//            // Update page table
//            entry.setFrameNumber(frameNumber);
//
//			//removeFromBackingStore(processID, pageNumber); // Remove from backing store
//            pagedIn++;
//
//            // Add to FIFO queue
//            fifoQueue.push({ processID, pageNumber });
//        }
//    }
//
//    return true;
//}

bool MemoryManager::loadPagesForProcess(std::shared_ptr<Process> process) {

    int processID = process->getPID();
    auto pageTable = process->getPageTable();
    if (!pageTable) return false;

    int requiredPages = pageTable->size();
	int availableFrames = memory.size();

    if (requiredPages > availableFrames) {
        pagedIn += requiredPages;
		pagedOut += requiredPages;
		return false; // Not enough frames available for the process
    }

    for (int pageNumber = 0; pageNumber < pageTable->size(); ++pageNumber) {
        PageEntry& entry = (*pageTable)[pageNumber];

        if (entry.isEmpty()) continue;

        if (!entry.isPageValid()) {
            int frameNumber = -1;

            if (hasFreeFrame()) {
                frameNumber = allocateFrame();
                memory[frameNumber] = FrameEntry{ processID, pageNumber, true };
            }
            else {
                frameNumber = evictPageFIFO(processID, pageNumber);
                if (frameNumber == -1) return false;
            }

            entry.setFrameNumber(frameNumber);
            pagedIn++;
            fifoQueue.push({ processID, pageNumber });
        }
    }

    return true;
}


int MemoryManager::evictPageFIFO(int processID, int pageNumber) {
    std::lock_guard<std::mutex> lock(memoryMutex);  // Lock acquired here
    if (fifoQueue.empty()) return -1;

    auto victim = fifoQueue.front();
    fifoQueue.pop();

    int victimPID = victim.first;
    int victimPage = victim.second;

    int evictFrame = -1;
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].getProcessID() == victimPID &&
            memory[i].getPageNumber() == victimPage) {
            evictFrame = i;
            break;
        }
    }

    if (evictFrame == -1) return -1;

    // Write to backing store
    writeToBackingStore(victimPID, victimPage);
    pagedOut++;

    // Invalidate page in page table
    auto victimProcess = GlobalScheduler::getInstance()->getProcessByPID(victimPID);
    if (victimProcess) {
        auto victimPageTable = victimProcess->getPageTable();
        if (victimPageTable && victimPage >= 0 && victimPage < victimPageTable->size()) {
            (*victimPageTable)[victimPage].invalidatePage();
        }
    }

    // Overwrite frame
    memory[evictFrame] = FrameEntry{ processID, pageNumber, true };
    memoryMutex.unlock();
    return evictFrame;
}


void MemoryManager::handlePageFault(int processPID, PageEntry* pageEntry) {
    if (!pageEntry) return;

    int frameNum = -1;
    if (!hasFreeFrame()) {
        frameNum = evictPageFIFO(processPID, pageEntry->getPageNumber());
        if (frameNum == -1) return;
    }
    else {
        frameNum = allocateFrame();
        pagedIn++;
    }

    loadFromBackingStore(processPID, pageEntry->getPageNumber());
    pageEntry->setFrameNumber(frameNum);
}

bool MemoryManager::hasFreeFrame() const {
    return std::any_of(memory.begin(), memory.end(), [](const FrameEntry& f) {
        return !f.isFrameValid();
    });
}

int MemoryManager::countFreeFrames() const {
    int count = 0;
    for (const auto& frame : memory) {
        if (!frame.isFrameValid()) {
            count++;
        }
    }
    return count;
}


int MemoryManager::allocateFrame() {
    for (int i = 0; i < memory.size(); ++i) {
        if (!memory[i].isFrameValid()) {
            return i; // Return the index of the first free frame
        }
    }
    return -1; // No free frame found
}

bool MemoryManager::deallocateMemory(int processID)
{
    bool deallocated = false;
    for (auto& frame : memory) {
        if (frame.getProcessID() == processID && frame.isFrameValid()) {
            frame.setState(false);
            frame.setProcessID(-1);
            frame.setPageNumber(-1);
            deallocated = true;
        }
    }

    // Remove from FIFO queue
    std::queue<std::pair<int, int>> tempQueue;
    while (!fifoQueue.empty()) {
        auto entry = fifoQueue.front();
        fifoQueue.pop();
        if (entry.first != processID) {
            tempQueue.push(entry);
        }
    }
    fifoQueue = tempQueue;

    return deallocated;
}

void MemoryManager::writeToBackingStore(int processID, int pageNumber)
{
    std::ofstream file("csopesy-backing-store.txt", std::ios::app);
    if (!file.is_open()) return;
    file << processID << ":" << pageNumber << "\n";
    file.close();
}

std::string MemoryManager::loadFromBackingStore(int processID, int pageNumber) {
    std::ifstream inFile("backing_store.txt");
    std::string line, result;
    std::string searchToken = std::to_string(processID) + ":" + std::to_string(pageNumber) + ":";

    while (std::getline(inFile, line)) {
        if (line.find(searchToken) == 0) {
            result = line.substr(searchToken.length());
            break;
        }
    }

    if (!result.empty()) {
        pagedIn++;
    }

    return result;
}

void MemoryManager::removeFromBackingStore(int processID, int pageNumber)
{
    std::ifstream inFile("csopesy-backing-store.txt");
    if (!inFile.is_open()) return;

    std::vector<std::string> remainingEntries;
    std::string line;
    std::string target = std::to_string(processID) + ":" + std::to_string(pageNumber);

    while (std::getline(inFile, line)) {
        if (line != target) {
            remainingEntries.push_back(line);
        }
    }
    inFile.close();

    std::ofstream outFile("csopesy-backing-store.txt", std::ios::trunc); // Overwrite the file
    for (const auto& entry : remainingEntries) {
        outFile << entry << "\n";
    }
    outFile.close();
}

void MemoryManager::clearBackingStore() // clear backing store when exiting operating system
{
    std::ofstream outFile("csopesy-backing-store.txt", std::ios::trunc);
    outFile.close();
}


int MemoryManager::getExternalFragmentation() const
{
    return getFreeMemory();
}

void MemoryManager::saveMemorySnapshot(int cycle) const 
{
    //std::string folder = "snapshots";
    //std::string filename = folder + "/memory_stamp_" + std::to_string(cycle) + ".txt";
    //std::ofstream file(filename);

    //if (!file.is_open()) {
    //    return;
    //}

    //// Get current time as string using std::chrono and localtime_s
    //auto now = std::chrono::system_clock::now();
    //std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    //std::tm tm;
    //localtime_s(&tm, &time_t_now);

    //char timeStr[64];
    //std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm);

    //file << "Timestamp: (" << timeStr << ") " << "\n";
    //file << "Processes in memory: " << getAllocatedProcessCount() << "\n";
    //file << "Total external fragmentation in KB: " << getExternalFragmentation() << "\n";
    //file << "Memory Snapshot:\n";

    //file << "---end--- = " << maxOverallMemory << "\n";
    //int addr = maxOverallMemory;
    //for (int i = memory.size() - 1; i >= 0; --i) {
    //    if (memory[i] != 0) {
    //        file << "\n";
    //        file << addr << "\n";
    //        file << "P" << memory[i] << "\n";
    //        addr -= memPerProc;
    //        file << addr << "\n";
    //    }
    //    else {
    //        addr -= memPerProc; // still decrease addr even if not printing
    //    }
    //}

    //file << "\n---start--- = 0\n";
}
int MemoryManager::getAllocatedProcessCount() const
{
    std::unordered_set<int> allocatedProcesses;
    for (const auto& frame : memory) {
        if (frame.isFrameValid()) {
            allocatedProcesses.insert(frame.getProcessID());
        }
    }
    return allocatedProcesses.size();
}

void MemoryManager::clearAllMemory() {
    for (int i = 0; i < memory.size(); ++i) {
        if (memory[i].isFrameValid()) {
            int pid = memory[i].getProcessID();
            int page = memory[i].getPageNumber();

            // Simulate writing page back to disk
            writeToBackingStore(pid, page);
            pagedOut++;
        }
        memory[i].freeFrame();
    }

    while (!fifoQueue.empty()) fifoQueue.pop();
}



std::string MemoryManager::getProcessSMI() const {
    std::ostringstream out;

    out << getCurrentTimestamp() << "\n";
    out << "===============================================================\n";
    out << "| PROCESS-SMI " << std::setw(45) << "CSOPESY Memory Usage " << std::setw(5) << "|\n";
    out << "===============================================================\n";

    // Memory summary
    int totalMem = getTotalMemory();
    int usedMem = getUsedMemory();
    int freeMem = getFreeMemory();
    double usagePercent = (static_cast<double>(usedMem) / totalMem) * 100.0;

    out << "| Memory Usage: " << std::setw(6) << usedMem << " / " << totalMem << " bytes ("
        << std::fixed << std::setprecision(1) << usagePercent << "%) "<< std::setw(21) <<"| \n";
    out << "| Free Memory: " << std::setw(7) << freeMem << " bytes" << std::setw(36) << "|\n";
    out << "===============================================================\n";

    // Process list header
    out << "| Processes:" << std::setw(52) << "|\n";
    out << "+-------+---------------+------------------+------------------+\n";
    out << "| PID   | Process Name  | Memory Used (B)  | Memory Used (KB) |\n";
    out << "+-------+---------------+------------------+------------------+\n";

    // Get process memory usage
    auto processUsage = getProcessMemoryUsage();
    if (processUsage.empty()) {
        out << "| No processes currently in memory" << std::setw(30) << "|\n";
    }
    else {
        for (const auto& proc : processUsage) {
            int pid = proc.first;
            int memUsed = proc.second;
            int memUsedKB = memUsed / 1024;

            // Get process name from GlobalScheduler
            std::string processName = "process_" + std::to_string(pid);
            if (GlobalScheduler::getInstance()) {
                auto process = GlobalScheduler::getInstance()->getProcessByPID(pid);
                if (process) {
                    processName = process->getName();
                }
            }

            out << "| " << std::setw(5) << pid
                << " | " << std::setw(13) << processName.substr(0, 13)
                << " | " << std::setw(16) << memUsed
                << " | " << std::setw(16) << memUsedKB << " |\n";
        }
    }

    out << "+-------+---------------+------------------+------------------+\n";
    out << "===============================================================\n";

    return out.str();
}

std::string MemoryManager::getVMStat() const {
    std::ostringstream out;

    out << getCurrentTimestamp() << "\n";
    out << "==========================================================\n";
    out << "|               Virtual Memory Statistics                |\n";
    out << "==========================================================\n";

    // Memory statistics
    int totalMem = getTotalMemory();
    int usedMem = getUsedMemory();
    int freeMem = getFreeMemory();

    out << "Memory Information:\n";
    out << "  Total Memory: " << std::setw(10) << totalMem << " bytes ("
        << (totalMem / 1024) << " KB)\n";
    out << "  Used Memory:  " << std::setw(10) << usedMem << " bytes ("
        << (usedMem / 1024) << " KB)\n";
    out << "  Free Memory:  " << std::setw(10) << freeMem << " bytes ("
        << (freeMem / 1024) << " KB)\n";
    out << "\n";

    // Page statistics
    int totalPages = getTotalPages();
    int usedPages = getUsedPages();
    int freePages = getFreePages();

    out << "Page Information:\n";
    out << "  Total Pages:  " << std::setw(10) << totalPages << " pages\n";
    out << "  Used Pages:   " << std::setw(10) << usedPages << " pages\n";
    out << "  Free Pages:   " << std::setw(10) << freePages << " pages\n";
    out << "  Page Size:    " << std::setw(10) << memPerFrame << " bytes\n";
    out << "\n";

    // Paging activity
    out << "Paging Activity:\n";
    out << "  Pages In:     " << std::setw(10) << getPagedIn() << " pages\n";
    out << "  Pages Out:    " << std::setw(10) << getPagedOut() << " pages\n";
    out << "\n";

	// System information
	int idleCPUTicks = CPUTick::getInstance()->getIdleCpuTicks();
	int activeCPUTicks = CPUTick::getInstance()->getActiveCpuTicks();

    out << "System Information:\n";
    out << "  Idle Ticks:    " << std::setw(10) << idleCPUTicks << " ticks\n";
    out << "  Active Ticks:  " << std::setw(10) << activeCPUTicks << " ticks\n";
    out << "  Total Ticks:   " << std::setw(10) << (idleCPUTicks + activeCPUTicks) << " ticks\n\n";


    // Process information
    int numProcesses = getNumProcessesInMemory();
    out << "Process Information:\n";
    out << "  Active Processes: " << std::setw(6) << numProcesses << " processes\n";

    // Get running vs waiting processes
    int runningProcesses = 0;
    int waitingProcesses = 0;
	int finishedProcesses = 0;

    if (GlobalScheduler::getInstance()) {
        for (int i = 0; i < GlobalScheduler::getInstance()->getProcessCount(); ++i) {
            auto& process = GlobalScheduler::getInstance()->getProcess(i);
            if (process->getState() == Process::RUNNING) {
                runningProcesses++;
            }
            else if (process->getState() == Process::WAITING || process->getState() == Process::READY) {
                waitingProcesses++;
            } 
            else if (process->getState() == Process::FINISHED) {
				finishedProcesses++;
            }
        }
    }

    out << "  Running:      " << std::setw(10) << runningProcesses << " processes\n";
    out << "  Waiting:      " << std::setw(10) << waitingProcesses << " processes\n";
	out << "  Finished:     " << std::setw(10) << finishedProcesses << " processes\n";
    out << "\n";

    // Detailed page frame information
    out << "Page Frame Details:\n";
    out << "+-------+----------+-----------+--------+\n";
    out << "| Frame | Process  | Page      | Status |\n";
    out << "+-------+----------+-----------+--------+\n";

    for (int i = 0; i < memory.size(); ++i) {
        const auto& frame = memory[i];
        out << "| " << std::setw(5) << i;

        if (frame.isFrameValid()) {
            out << " | " << std::setw(8) << frame.getProcessID()
                << " | " << std::setw(9) << frame.getPageNumber()
                << " | " << std::setw(6) << "USED";
        }
        else {
            out << " | " << std::setw(8) << "-"
                << " | " << std::setw(9) << "-"
                << " | " << std::setw(6) << "FREE";
        }
        out << " |\n";
    }

    out << "+-------+----------+-----------+--------+\n";

    return out.str();
}

int MemoryManager::getUsedMemory() const {
    int totalUsedBytes = 0;

    for (const auto& frame : memory) {
        if (frame.isFrameValid()) {
            int pid = frame.getProcessID();
            int pageNum = frame.getPageNumber();  

            auto process = GlobalScheduler::getInstance()->getProcessByPID(pid);
            if (process) {
                const auto& pageTable = process->getPageTable();
                if (pageNum >= 0 && pageNum < pageTable->size()) {
                    totalUsedBytes += (*pageTable)[pageNum].getUsedBytes();
                }
            }
        }
    }

    return totalUsedBytes;
}


int MemoryManager::getFreeMemory() const {
    return maxOverallMemory - getUsedMemory();
}

int MemoryManager::getNumProcessesInMemory() const {
    return getAllocatedProcessCount();
}

std::vector<std::pair<int, int>> MemoryManager::getProcessMemoryUsage() const {
    std::unordered_map<int, int> processUsedBytes;

    for (const auto& frame : memory) {
        if (frame.isFrameValid()) {
            int pid = frame.getProcessID();
            int pageNum = frame.getPageNumber();

            auto process = GlobalScheduler::getInstance()->getProcessByPID(pid);
            if (process) {
                const auto& pageTable = process->getPageTable();
                if (pageNum >= 0 && pageNum < pageTable->size()) {
                    processUsedBytes[pid] += (*pageTable)[pageNum].getUsedBytes();
                }
            }
        }
    }

    std::vector<std::pair<int, int>> result;
    for (const auto& entry : processUsedBytes) {
        result.push_back({ entry.first, entry.second });
    }

    std::sort(result.begin(), result.end());  // Sort by PID
    return result;
}


int MemoryManager::getUsedPages() const {
    int usedPages = 0;
    for (const auto& frame : memory) {
        if (frame.isFrameValid()) {
            usedPages++;
        }
    }
    return usedPages;
}

int MemoryManager::getFreePages() const {
    return numFrames - getUsedPages();
}

std::string MemoryManager::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time_t);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}

void MemoryManager::setMaxOverallMemory(int maxOverallMemory) 
{
	this->maxOverallMemory = maxOverallMemory;
}

void MemoryManager::setMemPerFrame(int memFrameSize) 
{
    this->memPerFrame = memFrameSize;
}

void MemoryManager::setMinMemPerProc(int minMemPerProc) 
{
    this->minMemPerProc = minMemPerProc;
}

void MemoryManager::setMaxMemPerProc(int maxMemPerProc) 
{
    this->maxMemPerProc = maxMemPerProc;
}

MemoryManager::MemoryManager(int maxOverallMemory, int memPerFrame, int minMemPerProc, int maxMemPerProc)
    : maxOverallMemory(maxOverallMemory), memPerFrame(memPerFrame), minMemPerProc(minMemPerProc), maxMemPerProc(maxMemPerProc)
{
	this->numFrames = maxOverallMemory / memPerFrame; // Calculate number of frames
    this->memory.resize(numFrames);
}

