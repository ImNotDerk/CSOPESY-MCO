#pragma once
#include <iostream>
#include <windows.h>
#include "Screen.h"
#include <string>
#include <iomanip>
#include <map>
typedef std::string String;

class ConsoleManager {
public:
	std::map<String, Screen> screenMaps;

	ConsoleManager() {
		
	}
};