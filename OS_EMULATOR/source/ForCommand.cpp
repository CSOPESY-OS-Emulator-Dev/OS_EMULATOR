#include "ForCommand.h"

ForCommand::ForCommand(int pid, std::vector<std::shared_ptr<ICommand>> instructions, int iterations) 
    : ICommand(pid, CommandType::FOR), instructions(std::move(instructions)), iterations(iterations), currentIteration(0), instructionIndex(0) {
}

void ForCommand::execute(Process& process, std::string timeExecuted, int coreID) {
    if (currentIteration < iterations) {
        if (instructionIndex < instructions.size()) {
            // Output to log instruction execution within the for loop
            if (!instructions[instructionIndex]->isCommandExecuted()) {
                instructions[instructionIndex]->execute(process, timeExecuted, coreID);
            }
            if (instructions[instructionIndex]->isCommandExecuted()) {
                instructionIndex++;
            }
        } else {
            currentIteration++;
            instructionIndex = 0; // Reset instruction index for the next iteration
        }
    } else {
        process.progressCount++;
        isExecuted = true; // Mark the command as executed
    }
}