#include "ProcessConsole.h"

ProcessConsole::ProcessConsole(std::string processName, std::string timeCreated) : AConsole(processName)
{
    this-> currentProcess = GlobalScheduler::getInstance()->getProcessByName(processName); // Change to existing process

    this->processName = processName;
    this->processID = this->currentProcess->processID;
    this->processLogs = this->currentProcess->outputLog;

    this->timeCreated = timeCreated;

    initialize();

    // Initialize Commands
    this->commandList.push_back("process-smi");
    this->commandList.push_back("exit");
}

ProcessConsole::~ProcessConsole()
{

}

void ProcessConsole::draw()
{
    //Linux
    //std::system("clear");
    //std::system("cls");
    for(int i = 0; i < this->outputList.size(); i++){
        std::cout << this->outputList[i] << std::endl;
    }
}

void ProcessConsole::process(std::string input) 
{
    auto parsed = parseInput(input);
    this->outputList.clear();
    
    bool isvalid = false;
    if(parsed.command == "exit") {
        exitConsole();
        isvalid = true;
    }
    if(parsed.command == "-help") {
        this->outputList.push_back("These are the available commands :");
        this->outputList.push_back("process-smi\nhelp\nexit");
        isvalid = true;
    }
    if(parsed.command == "process-smi") {
        initialize();
        isvalid = true;
    }
    if (!isvalid){
        this->outputList.push_back("Enter a Valid Command");
    }
}

void ProcessConsole::initialize()
{
    // Clear outputList
    this->outputList.clear();

    // Initialize Header (13 Lines)
    this->outputList.push_back("\nCreated: " + this->timeCreated);

    this->outputList.push_back("\nProcess name: " + this->processName);
    this->outputList.push_back("ID: " + this->processID);
    this->outputList.push_back("Logs:");

    for (const std::string& log : *(this->processLogs)) {
        this->outputList.push_back(log);
    }

    if (this->currentProcess.get()->currentState == FINISHED) {
        this->outputList.push_back("\nFinished!\n");
    } else {
        this->outputList.push_back("\nCurrent Instruction Line: " + std::to_string(this->currentProcess->progressCount));
        this->outputList.push_back("Lines of code: " + std::to_string(this->currentProcess->instructionCount) + '\n');
    }
    this->outputList.push_back("<-help> to view all available commands");
}

/*  The following are function definitions that executes each available commands
    Note: Only function definitions of commands are implemented below
*/

void ProcessConsole::exitConsole() {
    ConsoleManager::getInstance()->returnToPreviousConsole();
}