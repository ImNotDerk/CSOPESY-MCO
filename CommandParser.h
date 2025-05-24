#pragma once
#include "Screen.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>
#include <map>

class CommandParser {
public:
    CommandParser() = default;
    ~CommandParser() = default;

    std::map<std::string, Screen>& getScreenMaps() {
        return screenMaps;
    }

    void parse(const std::string& input);

private:
    std::map<String, Screen> screenMaps;
    void handleScreenCommand(const std::string& option, const std::string& name);
    void printTimestamp();
};
