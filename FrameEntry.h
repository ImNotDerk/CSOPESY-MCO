#pragma once
class FrameEntry
{
public:
	FrameEntry(); // Default constructor
	FrameEntry(int processID, int pageNumber, bool isValid);

	void setProcessID(int processID);
	void setPageNumber(int pageNumber);
	void setState(bool isValid);

	int getProcessID() const;
	int getPageNumber() const;
	bool isFrameValid() const;

	void freeFrame();

private:
	int processID;
	int pageNumber;
	bool isValid; // is occupied or not
};

