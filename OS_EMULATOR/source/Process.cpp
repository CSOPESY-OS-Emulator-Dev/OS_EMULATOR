#include "Process.h"

void Process::executeInstruction()
{
    if (this->currentInstruction < this->instructionCount)
    {
        // Execute the current instruction
        this->instructionList[this->currentInstruction]->execute(*this, getFormattedCurrentTime(), coreID);
        if (this->instructionList[this->currentInstruction]->isCommandExecuted())
            // std::cout << "Executed instruction: " << this->instructionList[this->currentInstruction]->getCommandType() << " for process: " << this->name << std::endl;
            this->currentInstruction++;

        if (this->progressCount == this->instructionCount)
        {
            this->setState(FINISHED);
            this->timeFinished = getFormattedCurrentTime();
        }
    }
}

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

int Process::getCurrentLine()
{
    return this->progressCount;
}

int Process::getProcessID()
{
    return this->processID;
}

std::string Process::getProcessName()
{
    return this->name;
}

void Process::addInstruction(std::shared_ptr<ICommand> instruction)
{
    // Add the instruction to the instruction list
    this->instructionList.push_back(instruction);
}

void Process::incrementInstructionCount(int count)
{
    // Increment the instruction count by the given count
    this->instructionCount += count;
}

Process::Process(std::string name, int id)
{
    // Initialize the instruction list and symbol table
    this->instructionList = std::vector<std::shared_ptr<ICommand>>();
    this->symbolTable = std::unordered_map<std::string, uint16_t>();
    this->outputLog = std::make_shared<std::vector<std::string>>();

    this->name = name;
    this->processID = id;

    this->progressCount = 0;
    this->instructionCount = 0;
    this->currentInstruction = 0;

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

void Process::writeToTxtFile()
{
    // Create a text file with the process name
    std::string filename = name + ".txt";
    std::ofstream MyFile(filename);

    // Check if the file was opened successfully
    if (!MyFile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write the process name and logs to the file
    MyFile << "Process name: " << name << "\n"; // change variable to what

    // Write each log entry to the file
    if (outputLog->empty())
    {
        MyFile << "No logs available.\n";
    }
    else
    {
        MyFile << "Logs:\n\n";

        // Iterate through the output log and write each entry
        for (const auto &outputLog : *outputLog)
        {
            MyFile << outputLog << "\n";
        }
    }

    MyFile.close();
}
