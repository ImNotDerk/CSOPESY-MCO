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
		std::cerr << "No previous console to return to." << std::endl;
		return;
	}

	//this->consoleTable[screenRef->getName()] = screenRef;

}

//void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
//{
//	if (this->consoleTable.contains(screenRef->getName()))
//	{
//		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use a different name." << std::endl;
//	}
//
//	this->consoleTable[screenRef->getName()] = screenRef;
//}

void ConsoleManager::switchToScreen(String screenName)
{
	if(this->consoleTable.contains(screenName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Screen name '" << screenName << "' not found." << std::endl;
	}
}

void ConsoleManager::exitApplication()
{
	this->running = false;
}

bool ConsoleManager::isRunning() const
{
	return this->running;
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	/*const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
	const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
	const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>*/

	this->consoleTable["MAIN_CONSOLE"] = mainConsole;
	/*this->consoleTable["MARQUEE_CONSOLE"] = marqueeConsole;
	this->consoleTable["SCHEDULING_CONSOLE"] = schedulingConsole;
	this->consoleTable["MEMORY_CONSOLE"] = memoryConsole;*/

	this->switchConsole(MAIN_CONSOLE);


}


