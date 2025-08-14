#include "ReadCommand.h"
#include "ConfigReader.h"
#include "iostream"

// implement logic to check whether memory block is initialized, if not read returns 0
ReadCommand::ReadCommand(const String& varName, uint16_t address,
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable,
    std::shared_ptr<std::map<int, PageEntry>> pageTable)
    : ICommand(processID, ICommand::READ), varName(varName), address(address),
    value(0), symbolTable(symbolTable), pageTable(pageTable) {
}

std::shared_ptr<ICommand> ReadCommand::clone() const {
    return std::make_shared<ReadCommand>(*this);
}

void ReadCommand::execute()
{
    ICommand::execute();
    // Check if address is valid
    if (!isValidAddress(this->address)) {
        value = 0; // Return 0 for invalid addresses
        return;
    }

    // Get page number and offset from virtual address
    int pageNumber = getPageNumber(this->address);
    int pageOffset = getPageOffset(this->address);

    // Check if page exists in page table
    auto pageIt = pageTable->find(pageNumber);
    if (pageIt == pageTable->end()) {
        value = 0; // Page doesn't exist
        return;
    }

    PageEntry& pageEntry = pageIt->second;

    // Check if page is loaded in memory
    if (!pageEntry.isPageValid()) {
        // Handle page fault - load page into memory
        handlePageFault(pageNumber);

        // Check again if page is now valid
        if (!pageEntry.isPageValid()) {
            value = 0; // Still not loaded, return 0
            return;
        }
    }

    // At this point, the page should be in memory
    // For simulation purposes, we'll check if the variable exists in symbol table
    auto symbolIt = symbolTable->find(varName);
    if (symbolIt != symbolTable->end()) {
        value = symbolIt->second;
    }
    else {
        value = 0; // Variable not found, return 0
    }

    // Store the read value in symbol table for the variable
    (*symbolTable)[varName] = value;
}

uint16_t ReadCommand::getValue() const
{
    return value;
}

String ReadCommand::getOutput() const
{
    return "Read: " + varName + " from address 0x" +
        std::to_string(this->address) + " = " + std::to_string(value);
}

int ReadCommand::getPageNumber(int address) const {
    int memPerFrame = ConfigReader::getInstance()->getMemPerFrame();
    return address / memPerFrame;
}

int ReadCommand::getPageOffset(int address) const {
    int memPerFrame = ConfigReader::getInstance()->getMemPerFrame();
    return address % memPerFrame;
}

bool ReadCommand::isValidAddress(int address) const {
    // Check if address is within valid range
    return address >= 0;
}

void ReadCommand::handlePageFault(int pageNumber) {
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