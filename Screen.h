#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
typedef std::string String;

class Screen {
public:
	String screenName;
	int currentInstruction;
	int totalInstruction;
	time_t timeCreated;

	Screen(String screenName) {
		this->screenName = screenName;
		this->currentInstruction = 0;
		this->totalInstruction = 0;
		this->timeCreated = time(&timeCreated);
	}

	void displayScreen();


private:
	void printTimestamp();

};