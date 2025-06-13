#include "ConsoleManager.h"
#include <iostream>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new ConsoleManager();
	}
}

void ConsoleManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr) 
	{
		this->currentConsole->display();
	}
	else
	{
		cerr << "No console is currently set." << endl;
	}

}

void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->process();
	}
	else
	{
		cerr << "No console is currently set." << endl;
	}
}

void ConsoleManager::switchConsole(String consoleName)
{
	if (this->consoleTable.contains(consoleName))
	{
		system("cls");
		this->currentConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		cerr << "Console name '" << consoleName << "' not initialized." << endl;
	}
}

void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		system("cls");
		this->currentConsole = this->previousConsole;
		this->currentConsole->onEnabled();
	}
	else
	{
		cerr << "No previous console to return to." << endl;
	}
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	const shared_ptr<MainConsole> mainConsole = make_shared<MainConsole>();
	this->consoleTable["MAIN_CONSOLE"] = mainConsole;
	this->switchConsole(MAIN_CONSOLE);
}


