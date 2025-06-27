#pragma once
#include "ICommand.h"
#include "Process.h"

class DeclareCommand : public ICommand {
private:
    std::string variable;
    uint16_t value;
    int pid;

public:
    DeclareCommand(int pid, const std::string& variable, uint16_t value);
    void execute(Process& process, std::string timeExecuted, int coreID) override;
};