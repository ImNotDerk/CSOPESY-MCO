#include <iostream>
#include <string>
#include <windows.h>
#include "ConsoleManager.h"
typedef std::string String;

int main() {
	bool running = true;
	ConsoleManager::initialize();
	ConsoleManager::getInstance();
	
	while (running) {
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();

		//running = ConsoleManager::getInstance()->isRunning();
	}

	return 0;
}

