#pragma once
#include <vector>
#include <memory>
#include "ICommand.h"
#include "Process.h"

class ForCommand : public ICommand {
public:
    ForCommand(int pid, std::vector<std::shared_ptr<ICommand>> instructions, int iterations);
    // Execute the loop command by executing each instruction for the specified number of iterations
    void execute(Process& process, std::string timeExecuted, int coreID) override;
    friend class SchedulerTestThread; // Allow SchedulerTestThread to access private members
private:
    std::vector<std::shared_ptr<ICommand>> instructions; // Instructions to execute in the loop
    int iterations; // Number of iterations to execute the loop
    int currentIteration; // Current iteration count
    int instructionIndex; // Index of the current instruction being executed
};