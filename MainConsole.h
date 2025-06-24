#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include "AConsole.h"
#include "ConsoleManager.h"
#include "ConfigReader.h"

typedef std::string String;

class MainConsole : public AConsole
{
public:
	MainConsole();
	MainConsole(String name);
	~MainConsole() = default;

	String getName() const;
	void onEnabled() override;
	void display() override;
	void process() override;

	void printHeader() const;
	/*void initializeHandler() const;
	void scheduler_testHandler() const;
	void scheduler_stopHandler() const;*/
	void report_utilHandler() const;
	void processScreenHandler(const std::string& screencommand, const std::string& screenname) const;

	//String displayCPUUtil() const;
	//String displayRunning();
	//String displayFinished();
	//String displayProcessSMI();
	//String displayVMStat();

	friend class ConsoleManager;
};

