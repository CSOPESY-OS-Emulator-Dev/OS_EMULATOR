#include "Process.h"  // Include the header file for the Process class
// Ensure that Process.h contains the declaration of the Process class

int Process::getTotalIntstruction()
{
    return this->totalInstructions;
}

int Process::getCurrentLine()
{
    return this->currentInstruction;
}

int Process::getProcessID()
{
    return this->processID;
}

// Add the new methods to the Process class
size_t Process::getMemorySize() const
{
    return this->memorySize;
}

void *Process::getBaseAddress() const
{
    return this->baseAddress;
}

void Process::setBaseAddress(void *address)
{
    this->baseAddress = address;
}

Process::Process(std::string name, int id, int instructionCount, size_t memorySize, int coreID)
{
    this->name = name;
    this->processID = id;
    
    this->totalInstructions = instructionCount;
    this->currentInstruction = 1;

    this->currentState = READY;

    this->memorySize = memorySize;
    this->coreID = coreID;
}

Process::~Process(){}
