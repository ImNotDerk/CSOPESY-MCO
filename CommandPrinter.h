#pragma once
#include "ICommand.h"
#include <string>

using namespace std;
typedef std::string String;

class CommandPrinter : public ICommand
{
public:
    CommandPrinter(int pid, String& printCommand);
    void execute() override;
private:
    String printCommand;
    int pid;
};