#include "Process.h"

void Process::executeInstruction(){
    // Check if the current instruction is within the total instructions
    if (this->currentInstruction <= this->totalInstructions) {
        // Simulate executing an instruction
        std::string logEntry = "Executing instruction " + std::to_string(this->currentInstruction) + " of process " + this->name;
        std::cout << logEntry << std::endl;
        this->outputLog->push_back(logEntry);
        
        // Increment the current instruction line
        this->currentInstruction++;
    } else {
        // If all instructions are executed, set state to FINISHED
        this->setState(FINISHED);
    }
}

void Process::setState(state newState)
{
    this->currentState = newState;
}

state Process::getState() const {
    return this->currentState;
}

void Process::setCoreID(int coreID) {
    this->coreID = coreID;
}

int Process::getCoreID() const {
    return this->coreID;
}

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

