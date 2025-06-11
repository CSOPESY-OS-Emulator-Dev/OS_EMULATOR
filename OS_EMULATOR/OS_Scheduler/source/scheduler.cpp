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
    GlobalScheduler::getInstance()->initializeCores(4, 4);
    // Start the cores
    GlobalScheduler::getInstance()->runCores();
    // Start the scheduler
    GlobalScheduler::getInstance()->runScheduler();
    // Create dummy processes and add them to the scheduler
    for (int i = 0; i < 10; ++i) {
        auto process = std::make_shared<Process>("Process" + std::to_string(i), i, 5); // Each process has 5 instructions
        GlobalScheduler::getInstance()->addProcess(process);
    }
    // Run the scheduler for a while
    std::this_thread::sleep_for(std::chrono::seconds(10));
    // Print finished processes
    std::cout << "Finished Processes:" << std::endl;
    for (const auto& process : GlobalScheduler::getInstance()->getFinishedProcesses()) {
        std::cout << "Process ID: " << process->getProcessID() << ", Name: Process " << process->getProcessID() << std::endl;
    }
    // Destroy GlobalScheduler instance
    GlobalScheduler::destroy();
    std::cout << "OS Scheduler finished." << std::endl;
    return 0;
}