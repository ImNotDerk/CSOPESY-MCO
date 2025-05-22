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
        //else if () { // check map of names for dupes
        //    std::cerr << "Error: Screen name already exists." << std::endl;
        //}
        else {
            Screen newScreen(name);
            system("cls");
            newScreen.displayScreen();
        }
    } 
    else if (option == "-r") {
        if (name.empty()) {
            std::cerr << "Error: No name provided for screen." << std::endl;
        }
        //else if () {
        //    std::cerr << "Error: Screen name does not exist." << std::endl;
        //}
        else {
            // Retrieve and display the screen with the given name
            // Assuming you have a method to retrieve the screen by name
            Screen existingScreen(name);
            system("cls");
            existingScreen.displayScreen();
        }
    }
    else {
        std::cout << "Unknown option for screen: " << option << std::endl;
    }
    
}
