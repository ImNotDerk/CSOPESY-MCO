#pragma once
#include <string>
#include <memory>
#include <vector>
#include "ICommand.h"

typedef std::string String;

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

    // void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand() const;
    void moveToNextLine();

    // bool isFinished() const;
    //int getRemainingTime() const;
    // int getCommandCounter() const;
    // int getLinesOfCode() const;
    // int getPID() const;
    // int getCPUCoreID() const;
    // ProcessState getState() const;
    // String getName() const;

private:
    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
    int cpuCoreID = -1;
    ProcessState currentState;
    friend class ResourceEmulator;
};