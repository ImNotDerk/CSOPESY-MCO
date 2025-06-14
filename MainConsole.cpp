#include "MainConsole.h"
#include "BaseScreen.h"

MainConsole::MainConsole()
{
}

MainConsole::MainConsole(String name)
{
	this->name = name;
}

void MainConsole::onEnabled() {
	system("cls");
	printHeader();
}

void MainConsole::display() {
	onEnabled();
}

void MainConsole::printHeader() const{
	cout << "   ______     ______     ______     ______   ______     ______     __  __    " << endl;
	cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << endl;
	cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << endl;
	cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << endl;
	cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << endl;
	cout << "Hello, Welcome to CSOPESY command line!" << endl;
	cout << "Type 'exit' to quit, 'clear' to clear the screen" << endl;
}

void MainConsole::initializeHandler() const{
	cout << "initialize command recognized. Doing something." << endl;
}
void MainConsole::scheduler_testHandler() const{
	cout << "scheduler-test command recognized. Doing something." << endl;
}
void MainConsole::scheduler_stopHandler() const{
	cout << "scheduler-stop command recognized. Doing something." << endl;
}
void MainConsole::report_utilHandler() const{
	cout << "report-util command recognized. Doing something." << endl;
}

void MainConsole::process(){
	string commandInput;
	cout << "root:\\\\>";
	getline(cin, commandInput);

	stringstream ss(commandInput);
	string command, arg1, arg2;
	ss >> command >> arg1 >> arg2;

	if (command == "exit") {
		exit(0);
	}
	else if (command == "clear") {
		system("cls");
		printHeader();
	}
	else if (command == "initialize") {
		initializeHandler();
	}
	else if (command == "screen") {
		
	}
	else if (command == "scheduler-test") {
		scheduler_testHandler();
	}
	else if (command == "scheduler-stop") {
		scheduler_stopHandler();
	}
	else if (command == "report-util") {
		report_utilHandler();
	}
	else {
		cout << "Unknown command: " << commandInput << endl;
	}
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
