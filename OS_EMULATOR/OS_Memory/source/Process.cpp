#include "../header/Process.h"

int Process::getTotalInstruction() {
    return this->totalInstructions;
}

int Process::getCurrentLine() {
    return this->currentInstruction;
}

int Process::getProcessID() {
    return this->processID;
}

size_t Process::getMemorySize() const {
    return this->memorySize;
}

void* Process::getBaseAddress() const {
    return this->baseAddress;
}

void Process::setBaseAddress(void* address) {
    this->baseAddress = address;
}

Process::Process(std::string name, int id, int instructionCount, size_t memorySize, int coreID) {
    this->name = name;
    this->processID = id;
    this->totalInstructions = instructionCount;
    this->currentInstruction = 1;
    this->memorySize = memorySize;
    this->baseAddress = nullptr;
    this->coreID = coreID;
    this->currentState = READY;
}

Process::~Process() {}
