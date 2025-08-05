#pragma once
#include "AConsole.h"
#include "ConsoleManager.h"

class MainConsole : public AConsole
{
public:
    void draw() override;
    void process(std::string input) override;
    void initialize() override;

    MainConsole();
    ~MainConsole();

private:
    bool isinitialized = false;
    std::vector<std::string> outputList;
    int minIns, maxIns, minMem, maxMem;

    

    // Function prototype of available commands
    void showVMStat();
    void setScreen(std::string, std::string);
    void setScreenIns(std::string, std::string, std::vector<std::string>);
    void redrawScreen(std::string);
    void reportUtil();
    void startScheduler();
    void stopScheduler();
    std::vector<std::string> parseInstructions(const std::string &input);
    void showProcessSMI(std::string processName);
    void showProcesses();
    void initializeOS();
};