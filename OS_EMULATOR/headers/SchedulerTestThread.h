#pragma once
#include <iostream>
#include <memory>
#include <random>
#include "IETThread.h"
#include "Process.h"
#include "GlobalScheduler.h"
#include "PrintCommand.h"
#include "ICommand.h"
#include "DeclareCommand.h"
#include "AddCommand.h"
#include "SubtractCommand.h"
#include "ForCommand.h"
#include <stack>

struct InstructionFrame {
    int remainingExecs;
    int nestingLevel;
    std::vector<std::shared_ptr<ICommand>> instructions;
    int iterations; // for FOR
    int expectedSubExecs; // for FOR
    bool isForContext;
};

class SchedulerTestThread : public IETThread {
    // SchedulerTestThread is a test thread for the scheduler.
    // It inherits from IETThread to allow running in a separate thread.
    // This class is used to test the scheduler's functionality and performance.
    // This class creates new processes and adds them to the scheduler's queue.
    // Processes are created every cpu cycle.
    // Process' instruction count range between minIns up to maxIns
    // Each instructions are randomly generated
public:
    SchedulerTestThread(int cpuCycle, int minIns, int maxIns);


    void run() override;
    void stop();
    void resume();

    // Create new process with a given name and ID
    std::shared_ptr<Process> createProcess(std::string processName);
    // Create a random instruction of a given command type
    std::shared_ptr<ICommand> createInstruction(CommandType commandType, int pid, std::string processName);
    // Return random command type
    std::vector<std::shared_ptr<ICommand>> generateInstructions(int& remainingExecs, int pid, const std::string& processName, int nestingLevel);
    std::vector<std::shared_ptr<ICommand>> generateInstructionsIterative(int &remainingExecs, int pid, const std::string &processName);
    // Get a random command type, with an option to include FOR command type
    CommandType getRandomCommandType(bool includeFOR); // Default includeFOR is true, to include FOR command type
    // Assign new process to scheduler
    void assignToScheduler(std::shared_ptr<Process> process);
    int getTotalExecutions(const std::vector<std::shared_ptr<ICommand>>& instructions);

private:
    int cpuTick; // The CPU tick for the test thread 
    int cpuCycle; // The CPU cycle for the test thread
    int minIns; // Minimum instructions per process
    int maxIns; // Maximum instructions per process

    bool isRunning = false; // Flag to control the thread's execution loop
    int processCount = 0; // Counter for the number of processes created
    int getRandNum(int min, int max);
};