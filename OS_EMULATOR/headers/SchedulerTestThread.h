#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <algorithm> // For std::min
#include "IETThread.h"
#include "Process.h"
#include "GlobalScheduler.h"
#include <stack>

enum CommandType {
    PRINT,
    DECLARE,
    ADD,
    SUBTRACT,
    SLEEP,
    FOR,
    TYPE_COUNT
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
    SchedulerTestThread(int cpuCycle, int minIns, int maxIns, int minMemProc, int maxMemProc, int memPerFrame);


    void run() override;
    void stop();
    void resume();

    // Create new process with a given name and memory size
    std::shared_ptr<Process> createProcess(std::string processName, int memorySize);
    // Create new process with a given name, memory size, and instructions
    std::shared_ptr<Process> createProcess(std::string processName, int memorySize, const std::vector<std::string> &lines);
    // Create a random instruction of a given command type
    std::string createInstruction(CommandType commandType, int pid, std::string processName);
    // Return random command type
    std::vector<std::string> generateInstructions(int totalExecs, const std::string& processName, int nestingLevel);
    // Get a random command type, with an option to include FOR command type
    CommandType getRandomCommandType(bool includeFOR, bool includeSLEEP); // Default includeFOR is true, to include FOR command type
    // Assign new process to scheduler
    void assignToScheduler(std::shared_ptr<Process> process);

private:
    int cpuTick; // The CPU tick for the test thread 
    int cpuCycle; // The CPU cycle for the test thread
    int minIns; // Minimum instructions per process
    int maxIns; // Maximum instructions per process
    int minMemProc; // Minimum memory per process
    int maxMemProc; // Maximum memory per process
    int memPerFrame; // Memory per frame

    bool isRunning = false; // Flag to control the thread's execution loop
    int processCount = 0; // Counter for the number of processes created
    int getRandNum(int min, int max);
};