#include "MainConsole.h"
#include "ConsoleManager.h"

void MainConsole::initialize()
{
    // Clear outputList
    this->outputList.clear();

    // Initialize Header (13 Lines)
    this->outputList.push_back(" ______ __  __  ______       ______  __    __  __  __  __      ______  ______ ______  ______    ");
    this->outputList.push_back("/\\__  _/\\ \\_\\ \\/\\  ___\\     /\\  ___\\/\\ \"-./  \\/\\ \\/\\ \\/\\ \\    /\\  __ \\/\\__  _/\\  __ \\/\\  == \\   ");
    this->outputList.push_back("\\/_/\\ \\\\ \\  __ \\ \\  __\\     \\ \\  __\\\\ \\ \\-./\\ \\ \\ \\_\\ \\ \\ \\___\\ \\  __ \\/_/\\ \\\\ \\ \\/\\ \\ \\  __<   ");
    this->outputList.push_back("   \\ \\_\\\\ \\_\\ \\_\\ \\_____\\    \\ \\_____\\ \\_\\ \\ \\_\\ \\_____\\ \\_____\\ \\_\\ \\_\\ \\ \\_\\\\ \\_____\\ \\_\\ \\_\\ ");
    this->outputList.push_back("    \\/_/ \\/_/\\/_/\\/_____/     \\/_____/\\/_/  \\/_/\\/_____/\\/_____/\\/_/\\/_/  \\/_/ \\/_____/\\/_/ /_/ ");
    // Borders
    this->outputList.push_back("\\=============================================================================================/");

    // Footer messages
    this->outputList.push_back("\t\t\t\tWelcome to CSOPESY commandline!");
    this->outputList.push_back("===============================================================================================\n");
    this->outputList.push_back(" Developers : ");
    this->outputList.push_back(" DEMANALATA, ASHANTIE LOUIZE BACO \n HILOMEN, GEO BRIAN \n KINTANAR, KRISTIAN ANTHONY REMEDIOS \n OLORES, SEAN ANDREI PAJARTIN\n");
    this->outputList.push_back("===============================================================================================\n");
    this->outputList.push_back("These are the available commands :");
    this->outputList.push_back("initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit\n");

}

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE")
{
    initialize();

    // Initialize Commands
    this->commandList.push_back("initialize");
    this->commandList.push_back("screen");
    this->commandList.push_back("scheduler-test");
    this->commandList.push_back("scheduler-stop");
    this->commandList.push_back("report-util");
    this->commandList.push_back("clear");
    this->commandList.push_back("exit");
}

MainConsole::~MainConsole()
{

}

void MainConsole::draw()
{
    //Linux
    std::system("clear");
    std::system("cls");
    for(int i = 0; i < this->outputList.size(); i++){
        std::cout << this->outputList[i] << std::endl;
    }
}

void MainConsole::process(std::string input) 
{
    auto parsed = parseInput(input);
    this->outputList.push_back("C:\\> " + input);
    
    for(int i = 0; i < this->commandList.size(); i++) {
        if(parsed.command == commandList[i]) {
            this->outputList.push_back(input + " command recognized. Doing something.");
        }
    }
    if(parsed.command == "exit") {
        ConsoleManager::getInstance()->exitApplication();
    }
    if(parsed.command == "clear") {
        initialize();
    }
}

