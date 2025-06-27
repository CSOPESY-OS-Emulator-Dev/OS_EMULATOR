#pragma once
#include "ICommand.h"
#include "Process.h"

class SleepCommand : public ICommand {
    // SleepCommand is a concrete implementation of ICommand that represents a sleep command.
    // It inherits from ICommand and implements the execute method to pause the process for a specified duration.
public:
    SleepCommand(int pid, int duration);
    void execute(Process& process, std::string timeExecuted, int coreID) override;
private:
    int duration; // Duration of sleep in milliseconds
};