#include "Process.h"

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

Process::Process(std::string name, int id, int instructionCount)
{
    this->outputLog = std::make_shared<std::vector<std::string>>();
    //this->outputLog->push_back("Hello World");
    
    this->name = name;
    this->processID = id;
    
    this->totalInstructions = instructionCount;
    this->currentInstruction = 1;

    this->currentState = READY;
}

Process::~Process(){}

