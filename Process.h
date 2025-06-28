#pragma once

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include <format>
#include <sstream>
#include <unordered_map>
#include <cstdlib>  
#include <iterator> 
#include <filesystem>
#include <algorithm>
#include <random>   

#include "ICommand.h"
#include "ConfigReader.h"
#include "PrintCommand.h"
#include "DeclareCommand.h"
#include "AddCommand.h"
#include "SubtractCommand.h"
#include "SleepCommand.h"
#include "ForCommand.h"

typedef std::string String;
typedef std::vector<std::shared_ptr<ICommand>> CommandList;
typedef std::unordered_map<std::string, uint16_t> Symbol_Table; // stores results from process commands

class Process {
public:
    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name);

    void addCommand(std::shared_ptr<ICommand> command);
    void executeCurrentCommand(int coreId); // called by ScheduleWorker

    bool isFinished() const;

    int incrementCommandCounter();

    int getRemainingTime() const;
    int getCommandCounter() const;
    std::size_t getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;

    void setState(ProcessState currentState);
    ProcessState getState() const;
    String getName() const;

    void generateRandomCommands();
    void printCommands() const;
    void logInstruction(int coreId, String message);

    void generateNestedForCommand(int currentDepth, int maxDepth, int repeats);

    CommandList getCommandList();
    String getRunningTimestamp();
    String getFinishedTimestamp();
    String getUniqueVariableName(); // used to store a unique var name in an unordered map
    std::vector<String> getLogs();

    String stateToString(ProcessState state);

    

private:
    int pid;
    String name;
    CommandList commandList;
    Symbol_Table symbolTable;

    int commandCounter;
    int commandCounterIndex;
    int cpuCoreID = -1;
    ProcessState currentState = WAITING;

    String runningTimestamp;
    String finishedTimestamp;

    std::vector<String> print_logs;

    String getCurrentTimestamp();
};
