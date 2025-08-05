#pragma once
#include "ICommand.h"
#include "PageEntry.h"
#include "MemoryManager.h"
#include <memory>
#include <unordered_map>
#include <map>

class WriteCommand : public ICommand
{
public:
    WriteCommand(uint16_t address, uint16_t value,
        std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable,
        std::shared_ptr<std::map<int, PageEntry>> pageTable);
    std::shared_ptr<ICommand> clone() const override;
    void execute() override;

    void reWrite();
    String getOutput() const override;

private:
    uint16_t address;
    String varName;
    uint16_t value;
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
    std::shared_ptr<std::map<int, PageEntry>> pageTable;
    bool writeSuccessful;

    int getPageNumber(int virtualAddr) const;
    int getPageOffset(int virtualAddr) const;
    bool isValidAddress(int virtualAddr) const;
    void handlePageFault(int pageNumber);
    void getUniqueVariableName();
};
