#pragma once
#include "AConsole.h"
#include "Process.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"

class ProcessConsole : public AConsole {
public:
    void draw() override;
    void process(std::string input) override;
    void initialize() override;    
    
    ProcessConsole(std::string processName, std::string timeCreated);
    ~ProcessConsole();

    friend class Process;

private:
    std::vector<std::string> outputList;
    std::vector<std::string> commandList;

    std::string processName;
    std::string processID;
    std::shared_ptr<std::vector<std::string>> processLogs;

    std::string timeCreated;

    std::shared_ptr<Process> currentProcess;

    void exitConsole();
};