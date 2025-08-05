#include "Process.h"

void Process::setState(state newState)
{
    this->currentState = newState;
}

state Process::getState() const
{
    return this->currentState;
}

void Process::setCoreID(int coreID)
{
    this->coreID = coreID;
    this->timeRunning = getFormattedCurrentTime();
}

int Process::getCoreID() const
{
    return this->coreID;
}

std::string Process::getTimeRunning()
{
    return this->timeRunning;
}

std::string Process::getTimeFinished()
{
    return this->timeFinished;
}

int Process::getTotalIntstruction()
{
    return this->instructionCount;
}

int Process::getProgressCount()
{
    return this->programCounter;
}

int Process::getProcessID()
{
    return this->processID;
}

std::string Process::getProcessName()
{
    return this->name;
}

size_t Process::getMemorySize() const
{
    return this->memorySize;
}

size_t Process::getByteSize() const
{
    return this->byteSize;
}

void Process::setByteSize(size_t byteSize)
{
    this->byteSize = byteSize;
}

void Process::setInstructions(std::vector<std::string> instructions)
{
    // Add the instructions to the instruction list
    this->instructionList.insert(instructionList.end(),instructions.begin(),instructions.end());
}

void Process::setTotalInstructions(int total)
{
    this->instructionCount = total;
}

Process::Process(std::string name, int id, int memorySize)
{
    // Initialize the instruction list and symbol table
    this->instructionList = std::vector<std::string>();
    this->outputLog = std::make_shared<std::vector<std::string>>();

    this->memorySize = memorySize;

    this->name = name;
    this->processID = id;

    this->progressCounter = 0;
    this->instructionCount = 0;

    this->currentState = READY;
    this->coreID = -1; // Default core ID, indicating no core assigned yet
}

std::string Process::getFormattedCurrentTime()
{
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();
    auto in_time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to local time
    std::tm* localTime = std::localtime(&in_time_t); // Use localtime_s on Windows

    // Format the output
    std::ostringstream oss;
    oss << "(" << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << ms.count()
        << ' ' << std::put_time(localTime, "%p") << ")";
    return oss.str();
}

Process::~Process() {}