#include "WriteCommand.h"

// implement logic to write a uint16_t value to the specified memory address
WriteCommand::WriteCommand(uint16_t address, uint16_t value,
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable,
    std::shared_ptr<std::map<int, PageEntry>> pageTable)
    : ICommand(processID, ICommand::WRITE), address(address),
    value(value), symbolTable(symbolTable), pageTable(pageTable), writeSuccessful(false) {
    this->varName = "";
}

std::shared_ptr<ICommand> WriteCommand::clone() const {
    return std::make_shared<WriteCommand>(*this);
}

void WriteCommand::execute()
{
    ICommand::execute();
    getUniqueVariableName();
    reWrite();
}

int WriteCommand::getPageNumber(int address) const {
    int memPerFrame = ConfigReader::getInstance()->getMemPerFrame();
    return address / memPerFrame;
}

int WriteCommand::getPageOffset(int address) const {
    int memPerFrame = ConfigReader::getInstance()->getMemPerFrame();
    return address % memPerFrame;
}

bool WriteCommand::isValidAddress(int address) const {
    // Check if address is within valid range
    return address >= 0;
}

void WriteCommand::reWrite()
{
    // Check if address is valid
    if (!isValidAddress(this->address)) {
        writeSuccessful = false;
        return;
    }

    // Get page number and offset from virtual address
    int pageNumber = getPageNumber(this->address);
    int pageOffset = getPageOffset(this->address);

    // Check if page exists in page table
    auto pageIt = pageTable->find(pageNumber);
    if (pageIt == pageTable->end()) {
        writeSuccessful = false;
        return;
    }

    PageEntry& pageEntry = pageIt->second;

    // Check if page is loaded in memory
    if (!pageEntry.isPageValid()) {
        // Handle page fault - load page into memory
        handlePageFault(pageNumber);

        // Check again if page is now valid
        if (!pageEntry.isPageValid()) {
            writeSuccessful = false;
            return;
        }
    }

    // At this point, the page should be in memory
    // Check if there's enough space in the page for the write operation
    if (!pageEntry.hasSpaceFor(sizeof(uint16_t))) {
        writeSuccessful = false;
        return;
    }

    // Perform the write operation
    // In a real implementation, you would write to the actual memory location
    // For simulation purposes, we'll store it in the symbol table
    if (!varName.empty()) {
        (*symbolTable)[varName] = value;
    }

    // You could also add the variable to the page entry if needed
    if (!varName.empty()) {
        pageEntry.addVariable(varName, value);
    }

    writeSuccessful = true;
}

String WriteCommand::getOutput() const
{
    if (writeSuccessful) {
        return "WRITE " + std::to_string(value) + " to address 0x" +
            std::to_string(this->address) +
            (!varName.empty() ? " (variable: " + varName + ")" : "") + " - SUCCESS";
    }
    else {
        return "WRITE " + std::to_string(value) + " to address 0x" +
            std::to_string(this->address) +
            (!varName.empty() ? " (variable: " + varName + ")" : "") + " - FAILED";
    }
}

void WriteCommand::handlePageFault(int pageNumber) {
    auto pageIt = pageTable->find(pageNumber);
    if (pageIt != pageTable->end()) {
        PageEntry* pageEntry = &pageIt->second;

        // Get the process ID (you might need to store this in the command)
        // For now, we'll use a placeholder - you should pass the actual process ID
        int processId = this->processID; // Make sure processID is set in constructor

        // Handle page fault through memory manager
        MemoryManager::getInstance()->handlePageFault(processId, pageEntry);
    }
}

void WriteCommand::getUniqueVariableName() { // generate a new variable name for the newly written value to address
    int varCounter = 0;
    String varTemp = "var";

    do {
        varName = varTemp + std::to_string(varCounter);
        varCounter++;
    } while (symbolTable->find(varName) != symbolTable->end());
}