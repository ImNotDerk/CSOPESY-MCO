#include "MainConsole.h"
#include "ConsoleManager.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
string response = "";
string commandMessage = "";
string outputArg2 = "";
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
			/*config.setParamList(this->fileName);*/
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
			// ConsoleManager::getInstance()->createBaseScreen(outputArg2);
			// int newCore = GlobalScheduler::getInstance()->getScheduler()->checkCoreQueue();
			// GlobalScheduler::getInstance()->getScheduler()->assignCore(GlobalScheduler::getInstance()->getMostRecentProcess(), newCore);
		}
		
		if (commandMessage == "screenLS")
		{
			/*std::cout << this->displayCPUUtil();
			std::cout << this->displayRunning();
			std::cout << this->displayFinished();*/
		}

		if (commandMessage == "schedule-test")
		{
			commandMessage = "";
			// GlobalScheduler::getInstance()->getScheduler()->schedulerStart();
		}

		if (commandMessage == "scheduler-stop")
		{
			commandMessage = "";
			// GlobalScheduler::getInstance()->getScheduler()->schedulerStop();
		}

		if (commandMessage == "report-util")
		{
			report_utilHandler();
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
		cout << "Console has not been initialized yet. Please type 'initialize' to start." << endl;
	}


}

void MainConsole::process() // this function handles the input from the user 
{
	string commandInput;
	cout << "root:\\\\>";
	getline(cin, commandInput);

	stringstream ss(commandInput);
	string command, arg1, arg2;
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
	else if (command == "scheduler-test") 
	{
		commandMessage = command;
	}
	else if (command == command)
	{
		commandMessage = command;
	}
	else if (command == "report-util") 
	{
		commandMessage = command;
		report_utilHandler();
	}
	else {
		commandMessage = "unknown-command";
		cout << "Unknown command: " << commandInput << endl;
	}
}

void MainConsole::printHeader() const
{
	cout << "   ______     ______     ______     ______   ______     ______     __  __    " << endl;
	cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << endl;
	cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << endl;
	cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << endl;
	cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << endl;
	cout << "Hello, Welcome to CSOPESY command line!" << endl;
	if (isInitialized == false) 
	{
		cout << "Type 'initialize' to initialize the console, 'exit' to quit" << endl;
	}
	else 
	{
		cout << "Type 'clear' to clear the screen, 'exit' to quit" << endl;
	}
}

void MainConsole::report_utilHandler() const {
	cout << "report-util command recognized. Doing something." << endl;
}

void MainConsole::processScreenHandler(const string& screencommand, const string& screenname) const{
	if (screencommand == "-s") {
		if (screenname.empty()) {
			cerr << "Error: No name provided for screen." << endl;
		}
		//else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
		//	std::cerr << "Error: Screen already exists. Please load instead." << std::endl;
		//}
		else {
			cout << "MAKE A PROCESS HERE" << endl;
		}
	}
	else if (screencommand == "-r") {
		if (screenname.empty()) {
			cerr << "Error: No name provided for screen." << endl;
		}
		//else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
		//	system("cls");
		//	getScreenMaps().at(name).displayScreen();
		//}
		else {
			cerr << "Error: Inputted screen name doesn't exists." << endl;
		}
	}
	else {
		cout << "Unknown option for screen: " << endl;
	}
}
