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
    // Constructor and destructor
    Process(std::string name, int id, int instructionCount, size_t memorySize = 0, int coreID = -1);
    ~Process();

    // Getters
    int getTotalInstruction();       // ✅ fixed spelling
    int getCurrentLine();
    int getProcessID();
    size_t getMemorySize() const;
    void* getBaseAddress() const;

    // Setter
    void setBaseAddress(void* address);

    friend class ProcessConsole;

private:
    std::string name;
    int processID;
    int coreID;

    int totalInstructions;      // ✅ matches with getTotalInstruction()
    int currentInstruction;

    size_t memorySize;
    void* baseAddress;

    state currentState;
};