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
    void addInstruction(std::shared_ptr<ICommand> instruction);

    Process(std::string name, int id);
    ~Process();

    friend class ProcessConsole;
    friend class PrintCommand;

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

    int instructionCount;
    int currentInstruction;

    state currentState;

    void writeToTxtFile(); // call it, maybe? process1Logs
};