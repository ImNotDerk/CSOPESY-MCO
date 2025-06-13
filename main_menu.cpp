#include <iostream>
#include <string>
#include <windows.h>
#include "CommandParser.h"
#include "ConsoleManager.h"
typedef std::string String;

static void printHeader() {
	std::cout << "   ______     ______     ______     ______   ______     ______     __  __    " << std::endl;
	std::cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << std::endl;
	std::cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << std::endl;
	std::cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << std::endl;
	std::cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << std::endl;
	std::cout << "Hello, Welcome to CSOPESY command line!" << std::endl;
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen" << std::endl;
}

static void initializeHandler() {
	std::cout << "initialize command recognized. Doing something." << std::endl;
}
static void scheduler_testHandler() {
	std::cout << "scheduler-test command recognized. Doing something." << std::endl;
}
static void scheduler_stopHandler() {
	std::cout << "scheduler-stop command recognized. Doing something." << std::endl;
}
static void report_utilHandler() {
	std::cout << "report-util command recognized. Doing something." << std::endl;
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
				std::cout << "Enter a command: ";
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

