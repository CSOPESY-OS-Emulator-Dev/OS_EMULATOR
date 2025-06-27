#include "ICommand.h"

ICommand::ICommand(int pid, CommandType commandType) : processID(pid), commandType(commandType) {
    // Constructor initializes the process ID and command type
}

CommandType ICommand::getCommandType() const {
    // Returns the type of command
    return commandType;
}

bool ICommand::isCommandExecuted() const
{
    return false;
}
