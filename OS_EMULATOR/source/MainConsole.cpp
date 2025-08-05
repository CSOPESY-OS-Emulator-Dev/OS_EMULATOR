#include "MainConsole.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include "MemoryManager.h"
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
    this->outputList.push_back("<-help> to view all available commands\n");   
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
    bool isvalid = false;
    auto parsed = parseInput(input);

    this->outputList.push_back("C:\\> " + input);

    if(parsed.command == "-help"&& isinitialized){
        this->outputList.push_back("These are the available commands :");
        this->outputList.push_back("initialize\nscreen -s <process name>\nscreen -r <process name>\nscheduler-start\nscheduler-stop\nreport-util\nclear\nexit\n");
        isvalid = true;
    }
    if(parsed.command == "-help" && !isinitialized){
        this->outputList.push_back("These are the available commands :");
        this->outputList.push_back("initialize\nclear\nexit\n");
        isvalid = true;
    }

    if(isinitialized){
        if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-s" ) {
            setScreen(parsed.args[1], parsed.args[2]);
        isvalid = true;
        }
        if(parsed.command == "screen" && parsed.args.size() == 2 && parsed.args[0] == "-r" ) {
            redrawScreen(parsed.args[1]);
            isvalid = true;
        }
        if(parsed.command == "screen" && parsed.args.size() == 1 && parsed.args[0] == "-ls" ) {
            showProcesses();
            isvalid = true;
        }
        if (parsed.command == "initialize" && isinitialized){
            this->outputList.push_back("The operating system is already initialized");
        }
        if (parsed.command == "scheduler-start"){
            startScheduler();
            isvalid = true;
        }
        if (parsed.command == "scheduler-stop"){
            stopScheduler();
            isvalid = true;
        }
        if (parsed.command == "report-util"){
            reportUtil();
            isvalid = true;
        }
    } else if(parsed.command == "initialize"){
        isinitialized = true;
        initializeOS();
        isvalid = true;
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
        isvalid = true;
    }
    if (!isvalid && isinitialized){
        this->outputList.push_back("Enter a Valid Command");
    }
}

void MainConsole::initializeOS()
{
    std::ifstream file("Config.txt");
    bool isValid = true;
    int num_cpu, quantum_cycles, batch_process_freq, min_ins, max_ins, delays_per_exec;
    // Memory parameters
    int max_overall_mem, mem_per_frame, min_mem_per_proc, max_mem_per_proc;
    std::string scheduler, input;
    this->outputList.push_back("------------------------------------");
    
    file >> input;
    if (input == "num-cpu")
    {
        file >> num_cpu;
        if(num_cpu >= 1 && num_cpu <= 128){
            this->outputList.push_back("num-cpu : " + std::to_string(num_cpu));
        }else{
            this->outputList.push_back("num-cpu : Invalid config");
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
    if (input == "quantum-cycles")
    {
        file >> quantum_cycles;
        if(quantum_cycles >= 1 && quantum_cycles <= 4294967296u){
            this->outputList.push_back("quantum-cycles : " + std::to_string(quantum_cycles));
        }else{
            this->outputList.push_back("quantum-cycles : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "batch-process-freq")
    {
        file >> batch_process_freq;
        if(batch_process_freq >= 1 && batch_process_freq <= 4294967296u){
            this->outputList.push_back("batch-process-freq : " + std::to_string(batch_process_freq));
        }else{
            this->outputList.push_back("batch-process-freq : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "min-ins")
    {
        file >> min_ins;
        if(min_ins >= 1 && min_ins <= 4294967296u){
            this->outputList.push_back("min-ins : " + std::to_string(min_ins));
        }else{
            this->outputList.push_back("min-ins : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "max-ins")
    {
        file >> max_ins;
        if(max_ins >= 1 && max_ins <= 4294967296u){
            this->outputList.push_back("max-ins : " + std::to_string(max_ins));
        }else{
            this->outputList.push_back("max-ins : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "delays-per-exec")
    {
        file >> delays_per_exec;
        // std::cout << delays_per_exec << std::endl;
        if(delays_per_exec >= 0 && delays_per_exec <= 4294967296u){
            this->outputList.push_back("delays-per-exec : " + std::to_string(delays_per_exec));
        }else{
            this->outputList.push_back("delays-per-exec : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }

    file >> input;
    if (input == "max-overall-mem")
    {
        file >> max_overall_mem;
        // std::cout << max_overall_mem << std::endl;
        if(max_overall_mem >= 64u && max_overall_mem <= 65536u){
            this->outputList.push_back("max-overall-mem : " + std::to_string(max_overall_mem));
        }else{
            this->outputList.push_back("max-overall-mem : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    
    file >> input;
    if (input == "mem-per-frame")
    {
        file >> mem_per_frame;
        // std::cout << mem_per_frame << std::endl;
        if(mem_per_frame >= 64u && mem_per_frame <= 65536u){
            this->outputList.push_back("mem-per-frame : " + std::to_string(mem_per_frame));
        }else{
            this->outputList.push_back("mem-per-frame : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    
    file >> input;
    if (input == "min-mem-per-proc")
    {
        file >> min_mem_per_proc;
        // std::cout << min_mem_per_proc << std::endl;
        if(min_mem_per_proc >= 64u && min_mem_per_proc <= 65536u){
            this->outputList.push_back("min-mem-per-proc : " + std::to_string(min_mem_per_proc));
        }else{
            this->outputList.push_back("min-mem-per-proc : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    
    file >> input;
    if (input == "max-mem-per-proc")
    {
        file >> max_mem_per_proc;
        // std::cout << max_mem_per_proc << std::endl;
        if(max_mem_per_proc >= 64u && max_mem_per_proc <= 65536u){
            this->outputList.push_back("max-mem-per-proc : " + std::to_string(max_mem_per_proc));
        }else{
            this->outputList.push_back("max-mem-per-proc : Invalid config");
            isinitialized = false;
            isValid=false;
        }
    }
    this->outputList.push_back("------------------------------------");
    if(max_ins<min_ins){
        isValid = false;
    }
    if(isValid){
        // Memory initialization
        MemoryManager::getInstance()->setMemoryManager(max_overall_mem,mem_per_frame,max_overall_mem/mem_per_frame);
        GlobalScheduler::getInstance()->initializeCores(num_cpu,delays_per_exec);
        GlobalScheduler::getInstance()->runCores();
        GlobalScheduler::getInstance()->setScheduler(scheduler,quantum_cycles);
        GlobalScheduler::getInstance()->runScheduler();
        GlobalScheduler::getInstance()->initializeProcessGeneration(batch_process_freq,min_ins,max_ins,min_mem_per_proc,max_mem_per_proc,mem_per_frame);

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

void MainConsole::setScreen(std::string processName, std::string memorySize)
{
    if (!ConsoleManager::getInstance()->registerConsole(processName, memorySize)) {
        this->outputList.push_back("Could not find " + processName + " console");
    }
}

void MainConsole::redrawScreen(std::string processName)
{
    if(!ConsoleManager::getInstance()->switchConsole(processName)){
        this->outputList.push_back("Could not find " + processName + " console");
    }
}

void MainConsole::showProcesses()
{
    this->outputList.push_back("\n------------------------------------");
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
        this->outputList.push_back("No finished processes.");
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
    out << GlobalScheduler::getInstance()->getCPUUtilization() << "\n";
    out << GlobalScheduler::getInstance()->getCoresUsed() << "\n";
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
    out << "\nFinished Processes:\n";
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

void MainConsole::showProcessSMI(std::string processName) {

    






    this->outputList.push_back("----------------------------------------------");
    this->outputList.push_back("| PROCESS-SMI V01.00 Driver Version: 0.1.00 |");
    this->outputList.push_back("----------------------------------------------");
    this->outputList.push_back(GlobalScheduler::getInstance()->getCPUUtilization());
    this->outputList.push_back("Memory Usage: " + 
                                std::to_string(MemoryManager::getInstance()->getMemoryUsage()) +
                                "/" +
                                std::to_string(MemoryManager::getInstance()->getMemorySize()) + 
                                "MiB");
    this->outputList.push_back("Memory Utilization: " + 
                                    std::to_string(MemoryManager::getInstance()->getMemoryUsage()/
                                    MemoryManager::getInstance()->getMemorySize() * 100) + "%"
                                );
    this->outputList.push_back("");
    this->outputList.push_back("==============================================");
    this->outputList.push_back("Running processes and memory usage:");
    this->outputList.push_back("----------------------------------------------");
    this->outputList.push_back("process05 134MiB");
    this->outputList.push_back("----------------------------------------------");
    


    









}