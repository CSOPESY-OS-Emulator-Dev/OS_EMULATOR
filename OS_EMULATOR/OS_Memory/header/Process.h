#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>

enum state {
    READY,
    RUNNING,
    WAITING,
    FINISHED
};

class Process {
public:
    int getTotalIntstruction();
    int getCurrentLine();
    int getProcessID();

    // Add these methods to the Process class
    size_t getMemorySize() const; // Add 
    void* getBaseAddress() const; // Add 
    void setBaseAddress(void* address); // Add 

    // Constructor and destructor                        // Add this parameter 
    Process(std::string name, int id, int instructionCount, size_t memorySize = 0, int coreID = -1);
    ~Process();

    friend class ProcessConsole;
    
private:
    std::string name;
    int processID;
    int coreID;

    int totalInstructions;
    int currentInstruction;

    // Add these members to the Process class
    size_t memorySize;
    void* baseAddress;

    state currentState;
};