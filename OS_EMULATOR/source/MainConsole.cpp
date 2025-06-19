#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"

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
    this->outputList.push_back("initialize\nscreen\nscreen -s <process name>\nscreen -r <process name>\nscheduler-test\nscheduler-stop\nreport-util\nclear\nexit\n");

}

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE")
{
    initialize();
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

    if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-s") {
        setScreen(parsed.args[1]);
    }
    if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-r") {
        redrawScreen(parsed.args[1]);
    }
    if(parsed.command == "screen" && parsed.args.size() == 1 && parsed.args[0] == "-ls") {
        showProcesses();
    }

    if(parsed.command == "exit") {
        ConsoleManager::getInstance()->exitApplication();
    }
    if(parsed.command == "clear") {
        initialize();
    }
}

/*  The following are function definitions that executes each available commands
    Note: Only function definitions of commands are implemented below
*/

void MainConsole::setScreen(std::string processName)
{
    ConsoleManager::getInstance()->registerConsole(processName);
}

void MainConsole::redrawScreen(std::string processName)
{
    if(ConsoleManager::getInstance()->switchConsole(processName));
        this->outputList.push_back("Could not find " + processName + " console");
}

void MainConsole::showProcesses()
{
    this->outputList.push_back("------------------------------------");
    this->outputList.push_back("Running Processes:");
    if (GlobalScheduler::getInstance()->getRunningProcesses().empty()) {
        this->outputList.push_back("No running processes.\n");
    } else {
        for (auto& string : GlobalScheduler::getInstance()->getRunningProcesses()) {
            this->outputList.push_back(string);
        }
    }
    // Print finished processes 
    this->outputList.push_back("\nFinished Processes:");
    if (GlobalScheduler::getInstance()->getFinishedProcesses().empty()) {
        this->outputList.push_back("No finished processes.\n");
    } else {
        for (auto& string : GlobalScheduler::getInstance()->getFinishedProcesses()) {
            this->outputList.push_back(string);
        }
    }
    this->outputList.push_back("------------------------------------\n");
}
