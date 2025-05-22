#include "Screen.h"


void Screen::printTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_s(&local_tm, &now_time);


    std::cout << "Timestamp of when the screen is created: ";
    std::cout << std::put_time(&local_tm, "%m/%d/%Y, %I:%M %p") << std::endl;

}

void Screen::displayScreen() {
	std::cout << "Screen Name: " << this->screenName << std::endl;
	std::cout << "Current Instruction: " << this->currentInstruction << std::endl;
	std::cout << "Total Instructions: " << this->totalInstruction << std::endl;
	printTimestamp();
}