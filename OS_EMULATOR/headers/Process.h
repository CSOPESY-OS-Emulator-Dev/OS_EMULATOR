#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "ICommand.h"

enum state
{
    READY,
    RUNNING,
    WAITING,
    FINISHED,
    SLEEPING
};

class Process
{
public:
    void executeInstruction();

    void setState(state newState);
    state getState() const;

    void setCoreID(int coreID);
    int getCoreID() const;

    std::string getTimeRunning();
    std::string getTimeFinished();

    int getTotalIntstruction();
    int getCurrentLine();
    int getProcessID();
    std::string getProcessName();

    // Used by the scheduler-test to pipe random instruction
    void addInstruction(std::shared_ptr<ICommand> instruction); // Default count is 1
    void incrementInstructionCount(int count = 1); // Increment instruction count by a given count, default is 1

    Process(std::string name, int id);
    ~Process();

    friend class ProcessConsole;
    friend class PrintCommand;
    friend class DeclareCommand;
    friend class AddCommand;
    friend class SubtractCommand;
    friend class SleepCommand;
    friend class ForCommand;
    friend class SchedulerTestThread;
private:
    std::string getFormattedCurrentTime();

    // To be implemented (Command Class)
    std::vector<std::shared_ptr<ICommand>> instructionList;
    std::unordered_map<std::string, uint16_t> symbolTable;

    // This vector is shared with the processConsole
    std::shared_ptr<std::vector<std::string>> outputLog;

    std::string timeRunning;
    std::string timeFinished;
    std::string name;

    int processID;
    int coreID;

    int progressCount;       // Total executed instructions (including nested)
    int currentInstruction;  // Index of current top-level instruction
    int instructionCount;
    int sleepDuration; // Duration in milliseconds for sleep state

    state currentState;

    void writeToTxtFile(); // call it, maybe? process1Logs
};