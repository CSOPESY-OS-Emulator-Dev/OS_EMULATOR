#include "PrintCommand.h"

PrintCommand::PrintCommand(int pid, const std::string& message)
    : ICommand(pid, PRINT), message(message), variable("") {
    // Constructor initializes the process ID, message string, and command type
}

PrintCommand::PrintCommand(int pid, const std::string& message, const std::string variable)
    : ICommand(pid, PRINT), message(message), variable(variable) {
    // Constructor initializes the process ID, message string, variable's value, and command type
    // Uses the associated process's symbol table in initializing value.
}

void PrintCommand::execute(Process& process, std::string timeExecuted, int coreID) {
    std::stringstream msg;
    auto it = process.symbolTable.find(this->variable);

    if (variable.empty()) {
        // If the variable is empty, just print the message
        msg << this->message;
    } else if (it != process.symbolTable.end()) {
        // If the variable exists in the symbol table, append its value to the message
        msg << it->second;
    } else {
        // If the variable does not exist and is empty, declare variable with value 0
        process.symbolTable[variable] = 0; // Initialize the variable in the symbol table
        msg << "0";
    }
    // Append the message to the process's output log
    process.outputLog->push_back(timeExecuted + " Core:" + std::to_string(coreID) + " \"" + msg.str() + "\"");
}