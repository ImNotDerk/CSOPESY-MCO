#pragma once
#include "Screen.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>

class CommandParser {
public:
    CommandParser() = default;
    ~CommandParser() = default;

    void parse(const std::string& input);

private:
    void handleScreenCommand(const std::string& option, const std::string& name);
    void printTimestamp();
};
