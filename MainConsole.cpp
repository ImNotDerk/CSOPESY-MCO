#include "MainConsole.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::string userInput = "";
std::string commandMessage = "";
std::string outputArg2 = "";
bool isInitialized = false;

MainConsole::MainConsole()
{
	this->name = "MainConsole";
}

MainConsole::MainConsole(String name)
{
	this->name = name;
}

void MainConsole::onEnabled() 
{
	system("cls");
	printHeader();
}

void MainConsole::display() // handles what displayes after the process function handles the input
{
	if (commandMessage.empty())
	{
		onEnabled();
	}

	if (isInitialized) {
		if (commandMessage == "initialize") 
		{
			ConfigReader::initialize();
			ConfigReader::getInstance()->setParams("config.txt");
			//ConfigReader::getInstance()->testPrint(); // for debugging only
			GlobalScheduler::initialize();
			GlobalScheduler::getInstance()->setCoreCount(ConfigReader::getInstance()->getNumCPU());
			GlobalScheduler::getInstance()->selectScheduler(ConfigReader::getInstance()->getSchedulerToUse());
			GlobalScheduler::getInstance()->getScheduler()->init();
			GlobalScheduler::getInstance()->getScheduler()->run();
			
			commandMessage = "";
		}

		if (commandMessage == "clear") 
		{
			system("cls");
			printHeader();
			commandMessage = "";
		} 

		if (commandMessage == "exit")
		{
			commandMessage = "";
			ConsoleManager::getInstance()->exitApplication();
		}

		if (commandMessage == "screenR")
		{
			commandMessage = "";
			ConsoleManager::getInstance()->switchToScreen(outputArg2);
		}

		if (commandMessage == "screenS")
		{
			commandMessage = "";
			std::shared_ptr<Process> newProcess = std::make_shared<Process>(
				ConsoleManager::getInstance()->getNumScreens(), outputArg2
			);
			ConsoleManager::getInstance()->createBaseScreen(newProcess, true);
			GlobalScheduler::getInstance()->addProcess(newProcess);
		}
		
		if (commandMessage == "screenLS")
		{
			this->screenLS = GlobalScheduler::getInstance()->getScheduler()->screenLS();
			std::cout << this->screenLS;
		}

		if (commandMessage == "scheduler-start")
		{
			commandMessage = "";
			GlobalScheduler::getInstance()->schedulerStart();
		}

		if (commandMessage == "scheduler-stop")
		{
			commandMessage = "";
			GlobalScheduler::getInstance()->schedulerStop();
		}

		if (commandMessage == "report-util")
		{
			std::string filename = "csopesy-log.txt";
			std::ofstream file(filename);
			file << this->screenLS;
			file.close();

			std::cout << "Report generated at " << fs::absolute(filename) << std::endl;

		}

		if (commandMessage == "show-ticks")
		{
			std::cout << CPUTick::getInstance()->getTicks() << std::endl;
		}

		if (commandMessage == "unknown-command")
		{
			commandMessage = "";
			std::cout << "Unknown command. Please try again." << std::endl;
		}
	} 

	else if (commandMessage == "exit")
	{
		commandMessage = "";
		ConsoleManager::getInstance()->exitApplication();
	}
	else 
	{
		std::cout << "Console has not been initialized yet. Please type 'initialize' to start." << std::endl;
	}


}

void MainConsole::process() // this function handles the input from the user 
{
	std::string commandInput;
	std::cout << "root:\\\\>";
	getline(std::cin, commandInput);

	std::stringstream ss(commandInput);
	std::string command, arg1, arg2;
	ss >> command >> arg1 >> arg2;

	if (command == "exit") 
	{
		commandMessage = command;
	}
	else if (command == "clear") 
	{
		commandMessage = command;
	}
	else if (command == "initialize") 
	{
		commandMessage = command;
		isInitialized = true;
	}
	else if (command == "screen" && arg1 == "-r" && !arg2.empty()) 
	{
		commandMessage = "screenR";
		outputArg2 = arg2;
	}
	else if (command == "screen" && arg1 == "-s" && !arg2.empty()) 
	{
		commandMessage = "screenS";
		outputArg2 = arg2;
	}
	else if (command == "screen" && arg1 == "-ls") 
	{
		commandMessage = "screenLS";
	}
	else if (command == "scheduler-start") 
	{
		commandMessage = command;
	}
	else if (command == "scheduler-stop")
	{
		commandMessage = command;
	}
	else if (command == "report-util") 
	{
		commandMessage = command;
	}
	else if (command == "show-ticks")
	{
		commandMessage = command;
	}
	else {
		commandMessage = "unknown-command";
	}
}

void MainConsole::printHeader() const
{
	std::cout << "   ______     ______     ______     ______   ______     ______     __  __    " << std::endl;
	std::cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << std::endl;
	std::cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << std::endl;
	std::cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << std::endl;
	std::cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << std::endl;
	std::cout << "Hello, Welcome to CSOPESY command line!" << std::endl;
	if (isInitialized == false) 
	{
		std::cout << "Type 'initialize' to initialize the console, 'exit' to quit" << std::endl;
	}
	else 
	{
		std::cout << "Type 'clear' to clear the screen, 'exit' to quit" << std::endl;
	}
}

void MainConsole::report_utilHandler() const {
	std::cout << "report-util command recognized. Doing something." << std::endl;
}

void MainConsole::processScreenHandler(const std::string& screencommand, const std::string& screenname) const{
	if (screencommand == "-s") {
		if (screenname.empty()) {
			std::cerr << "Error: No name provided for screen." << std::endl;
		}
		//else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
		//	std::cerr << "Error: Screen already exists. Please load instead." << std::endl;
		//}
		else {
			std::cout << "MAKE A PROCESS HERE" << std::endl;
		}
	}
	else if (screencommand == "-r") {
		if (screenname.empty()) {
			std::cerr << "Error: No name provided for screen." << std::endl;
		}
		//else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
		//	system("cls");
		//	getScreenMaps().at(name).displayScreen();
		//}
		else {
			std::cerr << "Error: Inputted screen name doesn't exists." << std::endl;
		}
	}
	else {
		std::cout << "Unknown option for screen: " << std::endl;
	}
}
