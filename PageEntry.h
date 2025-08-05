#pragma once

#include <cstdint>
#include <cmath>

#include "ICommand.h"
#include "ConfigReader.h"

struct SymbolEntry 
{
	uint16_t value;
	int addressOffset;
};

typedef std::unordered_map<std::string, SymbolEntry> VariableList;

class PageEntry
{
public:
	PageEntry();
	PageEntry(int pageNumber, int memPerPage);

	bool isPageValid() const; // check if page is valid
	int getFrameNumber() const; // get the frame number where the page is loaded in the physical memory
	int getUsedBytes() const; // get the number of bytes used in this page
	int getPageNumber() const;
	void setFrameNumber(int frameNumber); // set the frame number where the page is loaded in the physical memory
	void invalidatePage(); // invalidate the page

	bool hasSpaceFor(int bytes) const;
	bool addInstruction(std::shared_ptr<ICommand> instruction); // add instruction to this page
	bool addVariable(std::string varName, uint16_t value);

	int returnOffset(const std::string varName) const;

private:
	int memPerPage; // size of each page in bytes
	int usedBytes = 0;

	int pageNumber; // page number in the process
	bool isLoaded; // if page is loaded in memory
	int frameNumber; // frame number in memory where the page is loaded
	std::vector<std::shared_ptr<ICommand>> instructionList; // instructions associated with this page
	VariableList variableList;
	
	// If the memory contains 8192 bytes, then each page will be 256 bytes
	// if this is page 1, then memory address will be from 0x0000 to 0x0256
	// Offset will be calculated by if(pageNumber == 1) return usedBytes; else return pageNumber * PAGE_SIZE + usedBytes
};

