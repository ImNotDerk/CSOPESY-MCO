#include <iostream>
#include <string>
#include <windows.h>
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
static void screenHandler() {
	cout << "screen command recognized. Doing something." << endl;
}
static void scheduler_testHandler() {
	cout << "scheduler-test command recognized. Doing something." << endl;
}
static void scheduler_stopHandler() {
	cout << "scheduler-stop command recognized. Doing something." << endl;
}
static void report_utilHandler() {
	cout << "report-util command recognized. Doing something." << endl;
}

static bool commandHandler(string command) {
	if (command == "initialize") {
		initializeHandler();
	} else if (command == "screen") {
		screenHandler();
	} else if (command == "scheduler-test") {
		scheduler_testHandler();
	} else if (command == "scheduler-stop") {
		scheduler_stopHandler();
	} else if (command == "report-util") {
		report_utilHandler();
	} else if (command == "clear") {
		system("cls"); // Clear the screen
		printHeader(); // Reprint the header after clearing the screen
	} else if (command == "exit") {
		return false; // Exit the program
	} else {
		cout << "Unknown command: " << command << endl;
	}

	return true;
}

int main() {
	std::string commandInput;

	printHeader();
	
	while (true) {
		cout << "Enter a command: ";
		getline(cin, commandInput);
		if (!commandHandler(commandInput)) {
			break; // Exit the loop if commandHandler returns false
		}
	}

	return 0;
}

