#pragma once
#include <iostream>
#include <iomanip>
#include "ICommand.h"
#include "Process.h"

class PrintCommand : public ICommand {
    // PrintCommand is a concrete implementation of ICommand that represents a print command.
    // It inherits from ICommand and implements the execute method to print a message to the console.
public:
    PrintCommand(int pid, const std::string& message);
    PrintCommand(int pid, const std::string& message, const std::string variable);
    void execute(Process& process, std::string timeExecuted, int coreID) override;

private:
    std::string message; // The message to be printed
    std::string variable; // The variable to be printed, if any
};