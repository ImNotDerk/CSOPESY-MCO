#pragma once
#include <string>
#include <memory>
#include <vector>
#include <string>
#include "ICommand.h"
#include "ConfigReader.h"
#include "PrintCommand.h"
#include "DeclareCommand.h"
#include "AddCommand.h"
#include "SubtractCommand.h"
#include "SleepCommand.h"
#include "ForCommand.h"

typedef std::string String;

// Executes the list of commands in sequence
class Process
{
public:
    enum ProcessState
    {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name);

    void addCommand(std::shared_ptr<ICommand> command);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int incrementCommandCounter();

    int getRemainingTime() const;
    int getCommandCounter() const;
    std::size_t getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;
    ProcessState getState() const;
    String getName() const;

private:
    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
    int cpuCoreID = -1;
    ProcessState currentState;
    void generateRandomCommands();

    /*friend class ResourceEmulator;*/
};