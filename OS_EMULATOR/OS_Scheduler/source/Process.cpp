#include "Process.h"

void Process::executeInstruction(){
    // Check if the current instruction is within the total instructions
    if (this->currentInstruction < this->instructionCount) {
        // Simulate executing an instruction
        this->instructionList[this->currentInstruction]->execute(*this, getFormattedCurrentTime(), coreID);
        
        // Increment the current instruction line
        this->currentInstruction++;
        if (this->currentInstruction == this->instructionCount) {
            // If the current instruction exceeds total instructions, set state to FINISHED
            this->setState(FINISHED);
            this->timeFinished = getFormattedCurrentTime();
        }
    }
}

void Process::setState(state newState) {
    this->currentState = newState;
}

state Process::getState() const {
    return this->currentState;
}

void Process::setCoreID(int coreID) {
    this->coreID = coreID;
    this->timeRunning = getFormattedCurrentTime();
}

int Process::getCoreID() const {
    return this->coreID;
}

std::string Process::getTimeRunning() {
    return this->timeRunning;
}

std::string Process::getTimeFinished() {
    return this->timeFinished;
}

int Process::getTotalIntstruction() {
    return this->instructionCount;
}

int Process::getCurrentLine() {
    return this->currentInstruction;
}

int Process::getProcessID() {
    return this->processID;
}

std::string Process::getProcessName() {
    return this->name;
}

void Process::addInstruction(std::shared_ptr<ICommand> instruction) {
    // Add the instruction to the instruction list
    this->instructionList.push_back(instruction);
    this->instructionCount++;
}

Process::Process(std::string name, int id) {
    // Initialize the instruction list and symbol table
    this->instructionList = std::vector<std::shared_ptr<ICommand>>();
    this->symbolTable = std::unordered_map<std::string, uint16_t>();
    this->outputLog = std::make_shared<std::vector<std::string>>();
    
    this->name = name;
    this->processID = id;
    
    this->instructionCount = 0;
    this->currentInstruction = 0;

    this->currentState = READY;
    this->coreID = -1; // Default core ID, indicating no core assigned yet
}

std::string Process::getFormattedCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);  // For Windows use localtime_s

    std::ostringstream oss;
    oss << "(" << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S %p") << ")";
    return oss.str();
}

Process::~Process() {}
