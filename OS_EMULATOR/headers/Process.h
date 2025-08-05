#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map> 
#include <iomanip>
#include <chrono>
#include <ctime>
// #include "SymbolTable.h"

enum state {
    READY,
    RUNNING,
    WAITING,
    FINISHED,
    SLEEPING,
    ACCESSVIOLATION
};

struct Page {
    int virtualPageNumber;
    bool inMemory;
    int frameNumber; // Valid if inMemory is true
    bool dirty = false;
    bool referenced = false; // For LRU
};

struct Registers {
    uint16_t R1 = 0;
    uint16_t R2 = 0;
    uint16_t R3 = 0;

    void print() const {
        std::cout << "R1: " << R1 << " | R2: " << R2 << " | R3: " << R3 << '\n';
    }

    uint16_t& getRegister(int regId) {
        switch (regId) {
            case 0: return R1;
            case 1: return R2;
            case 2: return R3;
            default: throw std::runtime_error("Invalid register ID");
        }
    }
};

class Process {
private:
    std::string timeRunning;
    std::string timeFinished;
    std::string timeShutDowned;
    std::string name;
    std::string invalidAddress;

    int processID;
    int coreID;
    int instructionCount;

    size_t memorySize;
    size_t byteSize;
    state currentState;

    std::vector<std::string> instructionList;
    std::unordered_map<int, Page> pageTable; // virtualPage -> Page
    
public:
    std::string getFormattedCurrentTime();

    void setState(state newState);
    state getState() const;

    void setCoreID(int coreID);
    int getCoreID() const;

    std::string getTimeRunning();
    std::string getTimeFinished();
    std::string getTimeShutDowned();
    std::string getInvalidAddress();

    int getTotalIntstruction();
    int getProgressCount();
    int getProcessID();
    std::string getProcessName();

     // Add these methods to the Process class
    size_t getMemorySize() const; // Add 
    size_t getByteSize() const;
    void setByteSize(size_t byteSize);

    // Used by the scheduler-test to pipe random instructions
    void setInstructions(std::vector<std::string> instructions); // Default count is 1
    std::vector<std::string> getInstructions() const;
    void setTotalInstructions(int total);

    std::vector<std::string> stringLiterals;
    Registers registers;
    uint16_t programCounter = 64; // Virtual address above symbolTable
    int progressCounter = 0; // Instructions executed
    int sleepDuration; // Duration in milliseconds for sleep state
    bool hasBeenCompiled = false;
    // This vector is shared with the processConsole
    std::shared_ptr<std::vector<std::string>> outputLog;

    Process(std::string name, int id, int memorySize);
    ~Process();

    friend class ProcessConsole;
    friend class MemoryManager;
    friend class DiskManager;
    friend class SymbolTable;
    friend class CoreThread;
    friend class SchedulerTestThread;
};