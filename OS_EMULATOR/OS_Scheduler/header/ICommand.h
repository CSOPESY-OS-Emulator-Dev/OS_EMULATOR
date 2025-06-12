#pragma once
#include <iostream>

class Process; // Forward declaration of Process class

enum CommandType {
    PRINT,
    DECLARE,
    ADD,
    SUBTRACT,
    SLEEP,
    FOR,
    TYPE_COUNT
};

class ICommand {
    // ICommand is an interface for command classes that can be executed by a process.
    // It defines the basic structure for commands, including execution and string representation.
public:
    ICommand(int pid, CommandType commandType);
    // Get Command Type
    CommandType getCommandType() const;
    // Execute the command. This method should be implemented by derived classes to define the command's behavior.
    virtual void execute(Process& process, std::string timeExecuted, int coreID) = 0;
protected:
    int processID; // The ID of the process that owns this command
    CommandType commandType; // The type of command (e.g., PRINT, DECLARE, etc.)
};
