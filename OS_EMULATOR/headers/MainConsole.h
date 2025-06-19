#pragma once
#include "AConsole.h"
#include "ConsoleManager.h"

class MainConsole : public AConsole{
public:
    void draw() override;
    void process(std::string input) override;
    void initialize() override;
    
    MainConsole();
    ~MainConsole();

private:
    bool isinitialized = false;
    std::vector<std::string> outputList;
    std::vector<std::string> commandList;

    // Function prototype of available commands
    void setScreen(std::string);
    void redrawScreen(std::string);

    // New functions
    void showProcesses();
    void initializeOS();
};