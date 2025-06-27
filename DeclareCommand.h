#pragma once
#include "ICommand.h"
class DeclareCommand : public ICommand {
public:
    DeclareCommand(const String& varName, uint16_t value);
    void execute() override;

    void declareVariable(const String& varName);
    String getVariableName();
    uint16_t getValue();
    String getOutput() const override;
private:
    String var;
    uint16_t value;
};
