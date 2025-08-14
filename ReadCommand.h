#pragma once
#include "ICommand.h"
#include "PageEntry.h"
#include "MemoryManager.h"
#include <memory>
#include <unordered_map>
#include <map>

class ReadCommand : public ICommand
{
public:
	ReadCommand(const String& varName, uint16_t address,
		std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable,
		std::shared_ptr<std::map<int, PageEntry>> pageTable);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;

	uint16_t getValue() const;
	String getOutput() const override;


private:
	String varName;
	uint16_t address;
	uint16_t value;
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
	std::shared_ptr<std::map<int, PageEntry>> pageTable;

	int getPageNumber(int address) const;
	int getPageOffset(int address) const;
	bool isValidAddress(int address) const;
	void handlePageFault(int pageNumber);
};
