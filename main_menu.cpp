#include <iostream>
#include <string>
#include <windows.h>
#include "ConsoleManager.h"
typedef std::string String;

// This is the main entry point of the application

int main() {
	bool running = true;
	ConsoleManager::initialize();
	ConsoleManager::getInstance();
	
	while (running) {
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		running = ConsoleManager::getInstance()->isRunning();
	}

	ConsoleManager::destroy(); // Cleanup the console manager before exiting

	return 0;
}

