#include "ForCommand.h"

ForCommand::ForCommand(int pid, std::vector<std::shared_ptr<ICommand>> instructions, int iterations) 
    : ICommand(pid, CommandType::FOR), instructions(std::move(instructions)), iterations(iterations), currentIteration(0), instructionIndex(0) {
}

void ForCommand::execute(Process& process, std::string timeExecuted, int coreID) {
    if (currentIteration < iterations) {
        isExecuted = false;
        if (instructionIndex < instructions.size()) {
            // Output to log instruction execution within the for loop
            process.outputLog->push_back(timeExecuted + " Core:" + std::to_string(coreID) + " Executing instruction " + std::to_string(instructionIndex + 1) + " of " + std::to_string(instructions.size()) + " in FOR loop iteration " + std::to_string(currentIteration + 1) + ".");
            // Execute the current instruction
            instructions[instructionIndex]->execute(process, timeExecuted, coreID);
            if (instructions[instructionIndex]->isCommandExecuted()) {
                instructionIndex++;
            }
        } else {
            // Log the completion of the current iteration
            process.outputLog->push_back(timeExecuted + " Core:" + std::to_string(coreID) + " Completed iteration " + std::to_string(currentIteration + 1) + " of FOR loop with " + std::to_string(iterations) + " iterations.");
            process.progressCount++;
            currentIteration++;
            instructionIndex = 0; // Reset instruction index for the next iteration
        }
    } else {
        process.progressCount++;
        isExecuted = true; // Mark the command as executed
    }
}