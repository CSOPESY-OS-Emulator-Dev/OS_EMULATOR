#pragma once
#include <iostream>
#include <memory>
#include <vector>

enum state {
    READY,
    RUNNING,
    WAITING,
    FINISHED
};

class Process {
public:
    void executeInstruction();

    void setState(state newState);
    state getState() const;

    void setCoreID(int coreID);
    int getCoreID() const;
    
    int getTotalIntstruction();
    int getCurrentLine();
    int getProcessID();

    Process(std::string name, int id, int instructionCount);
    ~Process();

    friend class ProcessConsole;
private:
    // To be implemented (Command Class)
    // std::vector<ICommand> instructionList;
    // std::unordered_map<std::string, int16> symbolTable;

    // This vector is shared with the processConsole
    std::shared_ptr<std::vector<std::string>> outputLog;

    std::string name;
    int processID;
    int coreID;

    int totalInstructions;
    int currentInstruction;

    state currentState;
};