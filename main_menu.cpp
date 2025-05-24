#include <iostream>
#include <string>
#include <windows.h>
#include "CommandParser.h"
#include "ConsoleManager.h"
using namespace std;

static void printHeader() {
	cout << "   ______     ______     ______     ______   ______     ______     __  __    " << endl;
	cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << endl;
	cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << endl;
	cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << endl;
	cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << endl;
	cout << "Hello, Welcome to CSOPESY command line!" << endl;
	cout << "Type 'exit' to quit, 'clear' to clear the screen" << endl;
}

static void initializeHandler() {
	cout << "initialize command recognized. Doing something." << endl;
}
//static void screenHandler() {
//	cout << "screen command recognized. Doing something." << endl;
//}
static void scheduler_testHandler() {
	cout << "scheduler-test command recognized. Doing something." << endl;
}
static void scheduler_stopHandler() {
	cout << "scheduler-stop command recognized. Doing something." << endl;
}
static void report_utilHandler() {
	cout << "report-util command recognized. Doing something." << endl;
}

int main() {
	std::string commandInput;

	CommandParser parser; // create the parser instance

	printHeader();
	
	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, commandInput); 

		if (commandInput == "exit") {
			break;
		}
		else if (commandInput == "clear") {
			system("cls");
			printHeader();
		}
		else if (commandInput == "initialize") {
			initializeHandler();
		}
		else if (commandInput.rfind("screen", 0) == 0) {
			parser.parse(commandInput); // initial screen command

			// entering the screen mode loop
			std::string screenCommand;
			while (true) {
				std::cout << "[screen mode] Enter a command: ";
				std::getline(std::cin, screenCommand);

				if (screenCommand == "exit") {
					std::cout << "Exiting screen mode..." << std::endl;
					system("cls");
					printHeader();
					break;
				}
				else if (screenCommand.rfind("screen", 0) == 0) { // possible fix next time maybe
					parser.parse(screenCommand);
				}
				else {
					std::cout << "Invalid command. While in screen mode, only 'screen' commands are allowed or type 'exit' to leave." << std::endl;
				}
			}
		}
		else if (commandInput == "scheduler-test") {
			scheduler_testHandler();
		}
		else if (commandInput == "scheduler-stop") {
			scheduler_stopHandler();
		}
		else if (commandInput == "report-util") {
			report_utilHandler();
		}
		else {
			std::cout << "Unknown command: " << commandInput << std::endl;
		}
	}

	return 0;
}

