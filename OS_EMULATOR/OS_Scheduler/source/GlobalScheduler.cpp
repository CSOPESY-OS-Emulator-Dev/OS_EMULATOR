#include "GlobalScheduler.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler()
{
    // initialize scheduler algorithms
    schedulerAlgorithms["FCFS"] = std::make_shared<FCFSScheduler>(); // Example scheduler, replace with actual implementations
    //schedulerAlgorithms["RR"] = std::make_shared<AScheduler>(); // Example scheduler, replace with actual implementations
    currentScheduler = nullptr; // No scheduler set initially
}

GlobalScheduler *GlobalScheduler::getInstance() {
    return sharedInstance;
}

void GlobalScheduler::initialize() {
    // Initialize the GlobalScheduler instance
    if (sharedInstance == nullptr) {
        sharedInstance = new GlobalScheduler();
    }
}
void GlobalScheduler::destroy() {
    // Clean up the GlobalScheduler instance
    if (sharedInstance != nullptr) {
        // Stop all cores
        for (const auto& core : sharedInstance->cores) {
            core->stop();
        }
        // Stop the current scheduler
        if (sharedInstance->currentScheduler) {
            sharedInstance->currentScheduler->stop();
        }
        // Clear all resources
        sharedInstance->cores.clear();
        sharedInstance->schedulerAlgorithms.clear();
        sharedInstance->processes.clear();
        sharedInstance->finishedProcesses.clear();
        // Delete the shared instance
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

void GlobalScheduler::runCores() {
    // Start all core threads
    for (const auto& core : cores) {
        core->start();
    }
}

void GlobalScheduler::initializeCores(int numCores, int cpuCycle) {
    // Initialize the cores with the specified number and CPU cycle
    for (int i = 0; i < numCores; ++i) {
        auto core = std::make_shared<CoreThread>(i, cpuCycle);
        cores.push_back(core);
    }
}

void GlobalScheduler::runScheduler() {
    // Start the current scheduler
    currentScheduler->start();
}

void GlobalScheduler::setScheduler(std::string schedulerAlgorithm) {
    // Set the current scheduler based on the provided algorithm name
    auto it = schedulerAlgorithms.find(schedulerAlgorithm);
    if (it != schedulerAlgorithms.end()) {
        currentScheduler = it->second;
        std::cout << "Scheduler set to: " << schedulerAlgorithm << std::endl;
    } else {
        std::cerr << "Scheduler algorithm not found: " << schedulerAlgorithm << std::endl;
    }
}

void GlobalScheduler::finishProcess(std::shared_ptr<Process> process) {
    // Add the finished process to the list of finished processes
    std::lock_guard<std::mutex> lock(schedulerMutex); // Lock the mutex to ensure thread safety
    finishedProcesses.push_back(process);
    std::cout << "Process " << process->getProcessID() << " finished." << std::endl;
}

const std::vector<std::shared_ptr<Process>> &GlobalScheduler::getFinishedProcesses() const {
    return finishedProcesses; // Return the list of finished processes
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process) {
    // Add a process to the scheduler's ready queue
    std::lock_guard<std::mutex> lock(schedulerMutex); // Lock the mutex to ensure thread safety
    process->setState(READY); // Set process state to READY
    currentScheduler->addProcess(process); // Add the process to the current scheduler's queue
}