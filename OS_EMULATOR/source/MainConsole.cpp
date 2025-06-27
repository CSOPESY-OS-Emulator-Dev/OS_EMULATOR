#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include <chrono>
#include <ctime>
#include <iomanip>

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
    std::cout << std::flush;
    std::cout << "\033c";
    for (int i = 0; i < this->outputList.size(); i++)
    {
        std::cout << this->outputList[i] << std::endl;
    }
}

void MainConsole::process(std::string input)
{
    auto parsed = parseInput(input);

    this->outputList.push_back("C:\\> " + input);

    if(isinitialized){
        if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-s" ) {
        setScreen(parsed.args[1]);
        }
        if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-r" ) {
            redrawScreen(parsed.args[1]);
        }
        if(parsed.command == "screen" && parsed.args.size() == 1 && parsed.args[0] == "-ls" ) {
            showProcesses();
        }
        if (parsed.command == "initialize" && isinitialized){
            this->outputList.push_back("The operating system is already initialized");
        }
        if (parsed.command == "scheduler-start"){
            startScheduler();
        }
        if (parsed.command == "scheduler-stop"){
            stopScheduler();
        }
        if (parsed.command == "report-util"){
            reportUtil();
        }
    } else if(parsed.command == "initialize"){
        isinitialized = true;
        initializeOS();
    }
    else{
        this->outputList.push_back("Please initialize the operating system.");
    }

    if(parsed.command == "exit") {
        ConsoleManager::getInstance()->exitApplication();
    }
    if (parsed.command == "clear")
    {
        initialize();
    }
}

void MainConsole::initializeOS()
{
    std::ifstream file("Config.txt");
    bool isValid = true;
    int num_cpu, quantum_cycles, batch_process_freq, min_ins, max_ins, delays_per_exec;
    std::string scheduler, input;
    this->outputList.push_back("------------------------------------");
    
    file >> input;
    if (input == "num_cpu")
    {
        file >> num_cpu;
        if(num_cpu >= 1 && num_cpu <= 128){
            this->outputList.push_back("num_cpu : " + std::to_string(num_cpu));
        }else{
            this->outputList.push_back("num_cpu : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    file >> input;
    if (input == "scheduler"){
        file >> scheduler;
        if(scheduler == "rr" || scheduler == "fcfs"){
            this->outputList.push_back("scheduler : " + scheduler);
        }else{
            this->outputList.push_back("scheduler : Not supported Scheduling");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "quantum_cycles")
    {
        file >> quantum_cycles;
        if(quantum_cycles >= 1 && quantum_cycles <= 4294967296u){
            this->outputList.push_back("quantum_cycles : " + std::to_string(quantum_cycles));
        }else{
            this->outputList.push_back("quantum_cycles : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "batch_process_freq")
    {
        file >> batch_process_freq;
        if(batch_process_freq >= 1 && batch_process_freq <= 4294967296u){
            this->outputList.push_back("batch_process_freq : " + std::to_string(batch_process_freq));
        }else{
            this->outputList.push_back("batch_process_freq : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "min_ins")
    {
        file >> min_ins;
        if(min_ins >= 1 && min_ins <= 4294967296u){
            this->outputList.push_back("min_ins : " + std::to_string(min_ins));
        }else{
            this->outputList.push_back("min_ins : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "max_ins")
    {
        file >> max_ins;
        if(max_ins >= 1 && max_ins <= 4294967296u){
            this->outputList.push_back("max_ins : " + std::to_string(max_ins));
        }else{
            this->outputList.push_back("max_ins : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "delay_per_exec")
    {
        file >> delays_per_exec;
        if(delays_per_exec >= 0 && delays_per_exec <= 4294967296u){
            this->outputList.push_back("delays_per_exec : " + std::to_string(delays_per_exec));
        }else{
            this->outputList.push_back("delays_per_exec : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    
    this->outputList.push_back("------------------------------------");

    if(isValid){
        GlobalScheduler::getInstance()->initializeCores(num_cpu,delays_per_exec);
        GlobalScheduler::getInstance()->runCores();
        GlobalScheduler::getInstance()->setScheduler(scheduler,quantum_cycles);
        GlobalScheduler::getInstance()->runScheduler();
        GlobalScheduler::getInstance()->initializeProcessGeneration(batch_process_freq,min_ins,max_ins);

        this->outputList.push_back("OS Initialized");    
    }else{
        this->outputList.push_back("Invalid Config");
    }
    file.close();
    // need running the system
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
    if(ConsoleManager::getInstance()->switchConsole(processName)){
        this->outputList.push_back("Could not find " + processName + " console");
    }
}

void MainConsole::showProcesses()
{
    this->outputList.push_back(GlobalScheduler::getInstance()->getCPUUtilization());
    this->outputList.push_back(GlobalScheduler::getInstance()->getCoresUsed());
    this->outputList.push_back(GlobalScheduler::getInstance()->getCoresAvailable());
    
    this->outputList.push_back("------------------------------------");

    // Print running processes
    this->outputList.push_back("Running Processes:");
    if (GlobalScheduler::getInstance()->getRunningProcesses().empty())
    {
        this->outputList.push_back("No running processes.\n");
    }
    else
    {
        for (auto &string : GlobalScheduler::getInstance()->getRunningProcesses())
        {
            this->outputList.push_back(string);
        }
    }
    // Print finished processes
    this->outputList.push_back("\nFinished Processes:");
    if (GlobalScheduler::getInstance()->getFinishedProcesses().empty())
    {
        this->outputList.push_back("No finished processes.\n");
    }
    else
    {
        for (auto &string : GlobalScheduler::getInstance()->getFinishedProcesses())
        {
            this->outputList.push_back(string);
        }
    }
    this->outputList.push_back("------------------------------------\n");
}

void MainConsole::reportUtil()
{
    // Create a log file to store the report
    std::string filename = "csopesy-log.txt";
    std::ofstream out(filename);

    // Check if the file opened successfully
    if (!out.is_open())
    {
        this->outputList.push_back("Failed to open log file.");
        return;
    }

    // Add date and time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    out << "Log generated on: " << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << "\n";
    out << "------------------------------------\n";

    //auto scheduler = GlobalScheduler::getInstance();

    // Add CPU info at the top
    out << GlobalScheduler::getInstance()->getCPUUtilization();
    out << GlobalScheduler::getInstance()->getCoresUsed();
    out << GlobalScheduler::getInstance()->getCoresAvailable();
    out << "\n------------------------------------\n";

    // Running Processes
    out << "Running Processes:\n";
    auto running = GlobalScheduler::getInstance()->getRunningProcesses();
    if (running.empty())
    {
        out << "No running processes.\n";
    }
    else
    {
        for (const auto &line : running)
        {
            out << line << "\n";
        }
    }

    // Finished Processes
    out << "Finished Processes:\n";
    auto finished = GlobalScheduler::getInstance()->getFinishedProcesses();
    if (finished.empty())
    {
        out << "No finished processes.\n";
    }
    else
    {
        for (const auto &line : finished)
        {
            out << line << "\n";
        }
    }

    out << "------------------------------------\n";
    out.close();

    // Notify the user that the report has been generated
    this->outputList.push_back("Report generated at " + filename + "!");
}

void MainConsole::startScheduler() {
    GlobalScheduler::getInstance()->startProcessGeneration();
    this->outputList.push_back("Start Generating Processes");
}

void MainConsole::stopScheduler() {
    GlobalScheduler::getInstance()->stopProcessGeneration();
    this->outputList.push_back("Stop Generating Processes");
}
