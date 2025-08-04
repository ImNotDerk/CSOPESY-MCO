#include "PageEntry.h"

PageEntry::PageEntry(int pageNumber)
	: pageNumber(pageNumber), // or 0 or whatever default makes sense
	usedBytes(0),
	isLoaded(false),
	frameNumber(-1),
	instructionList(),
	variableList()
{
	// nothing else needed
}


bool PageEntry::isPageValid() const
{
	return this->isLoaded;
}

int PageEntry::getFrameNumber() const
{
	return this->frameNumber;
}

void PageEntry::invalidatePage()
{
	this->isLoaded = false; // Mark the page as invalid
	this->frameNumber = -1; // Reset frame number to indicate no frame is assigned
}

void PageEntry::setFrameNumber(int frameNumber)
{
	this->frameNumber = frameNumber;
	this->isLoaded = true; // Set the page as valid when a frame number is assigned
}

bool PageEntry::hasSpaceFor(int bytes) const
{
	return (this->usedBytes + bytes <= PAGE_SIZE);
}

bool PageEntry::addInstruction(std::shared_ptr<ICommand> instruction)
{
	int size = 0;

	switch (instruction->getCommandType())
	{
		case ICommand::READ:
		case ICommand::WRITE:
			size = 1;
			break;
		case ICommand::PRINT:
		case ICommand::DECLARE:
		case ICommand::SLEEP:
			size = 2;
			break;
		case ICommand::ADD:
		case ICommand::SUBTRACT:
			size = 3;
			break;
		default:
			return false; // Unsupported command type
	}
	if (hasSpaceFor(size))
	{
		instructionList.push_back(instruction);
		usedBytes += size;
		return true;
	}
	return false;
}

bool PageEntry::addVariable(std::string varName, uint16_t value)
{
	int size = 2;

	if (hasSpaceFor(size)) 
	{
		variableList[varName] = { value, usedBytes }; // sets the varName, value, and offset for address
		return true;
	}
	return false;
}

int PageEntry::returnOffset(std::string varName) const // for virtual memory
{
	auto it = variableList.find(varName);
	if (it != variableList.end()) {
		if (this->pageNumber == 1)
			return it->second.addressOffset;
		else
			return this->pageNumber * PAGE_SIZE + it->second.addressOffset;
	}
	else {
		// handle case where varName doesn't exist
		throw std::runtime_error("Variable not found in variableList.");
	}

}