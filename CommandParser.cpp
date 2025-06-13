#include "CommandParser.h"

void CommandParser::parse(const std::string& input) {
    std::istringstream iss(input);
    std::string command, option, name;
    iss >> command >> option >> name;

    if (command == "screen") {
        handleScreenCommand(option, name);
    }
    else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

void CommandParser::handleScreenCommand(const std::string& option, const std::string& name) {
    if (option == "-s") {
        if (name.empty()) {
            std::cerr << "Error: No name provided for screen." << std::endl;
        }
        else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
            std::cerr << "Error: Screen already exists. Please load instead." << std::endl;
        }
        else {
            Screen newScreen(name);
            this->getScreenMaps().insert({name, newScreen});
            system("cls");
            newScreen.displayScreen();
        }
    } 
    else if (option == "-r") {
        if (name.empty()) {
            std::cerr << "Error: No name provided for screen." << std::endl;
        }
        else if (this->getScreenMaps().find(name) != this->getScreenMaps().end()) {
            system("cls");
            getScreenMaps().at(name).displayScreen();
        }
        else {
            std::cerr << "Error: Inputted screen name doesn't exists." << std::endl;
        }
    }
    else if (option == "-ls") {
		std::cout << "LIST SCREEN HERE" << std::endl;
    }
    else {
        std::cout << "Unknown option for screen: " << option << std::endl;
    }  
}
