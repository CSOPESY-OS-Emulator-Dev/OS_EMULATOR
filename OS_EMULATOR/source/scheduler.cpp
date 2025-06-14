#include <iostream>
#include "GlobalScheduler.h"

int main() {
    std::cout << "OS Scheduler started." << std::endl;
    // TODO: Initialize and run scheduler logic here
    // Initialize GlobalScheduler
    GlobalScheduler::initialize();
    // Set the scheduler algorithm (e.g., "FCFS")
    GlobalScheduler::getInstance()->setScheduler("FCFS");
    // Initialize cores (e.g., 4 cores with a CPU cycle of 4)
    GlobalScheduler::getInstance()->initializeCores(4, 0);
    // Start the cores
    GlobalScheduler::getInstance()->runCores();
    // Start the scheduler
    GlobalScheduler::getInstance()->runScheduler();
    // Initialize process generator
    GlobalScheduler::getInstance()->initializeProcessGeneration(1, 100, 100);
    
    // Manually create 10 processes
    for (int i = 0; i < 10; i++) {
        GlobalScheduler::getInstance()->createProcess("Process_" + std::to_string(i));
    }
    
    // Start process generation
    //GlobalScheduler::getInstance()->startProcessGeneration();
    // Run the scheduler for a while
    std::this_thread::sleep_for(std::chrono::seconds(10));
    // Print finished processes
    // Stop process generation
    //GlobalScheduler::getInstance()->stopProcessGeneration();
    // Print running processes
    std::cout << "Running Processes:" << std::endl;
    for (auto& string : GlobalScheduler::getInstance()->getRunningProcesses()) {
        std::cout << string << std::endl;
    }
    // Print finished processes
    std::cout << "Finished Processes:" << std::endl;
    for (auto& string : GlobalScheduler::getInstance()->getFinishedProcesses()) {
        std::cout << string << std::endl;
    }

    // Destroy GlobalScheduler instance
    GlobalScheduler::destroy();
    std::cout << "OS Scheduler finished." << std::endl;
    return 0;
}