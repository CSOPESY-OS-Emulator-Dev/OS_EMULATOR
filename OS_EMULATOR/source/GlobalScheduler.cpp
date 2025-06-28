#include "GlobalScheduler.h"
#include "CoreThread.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "SchedulerTestThread.h"

GlobalScheduler *GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler()
{
    // initialize scheduler algorithms
    schedulerAlgorithms["fcfs"] = std::make_shared<FCFSScheduler>();
    schedulerAlgorithms["rr"] = std::make_shared<RRScheduler>();
    currentScheduler = nullptr; // No scheduler set initially
}

GlobalScheduler *GlobalScheduler::getInstance()
{
    return sharedInstance;
}

void GlobalScheduler::initialize()
{
    // Initialize the GlobalScheduler instance
    if (sharedInstance == nullptr)
    {
        sharedInstance = new GlobalScheduler();
    }
}

void GlobalScheduler::destroy()
{
    // Clean up the GlobalScheduler instance
    if (sharedInstance != nullptr)
    {
        // Stop the process generator if it exists
        if (sharedInstance->processGenerator)
        {
            sharedInstance->processGenerator->stop();
        }
        // Stop all cores
        for (const auto &core : sharedInstance->cores)
        {
            core->stop();
        }
        // Stop the current scheduler
        if (sharedInstance->currentScheduler)
        {
            sharedInstance->currentScheduler->stop();
        }
        // Clear all resources
        sharedInstance->cores.clear();
        sharedInstance->schedulerAlgorithms.clear();
        sharedInstance->processMap.clear();
        sharedInstance->finishedProcesses.clear();
        // Delete the shared instance
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

void GlobalScheduler::runCores()
{
    // Start all core threads
    for (const auto &core : cores)
    {
        core->start();
    }
}

void GlobalScheduler::initializeCores(int numCores, int cpuCycle)
{
    // Initialize the cores with the specified number and CPU cycle
    for (int i = 0; i < numCores; ++i)
    {
        auto core = std::make_shared<CoreThread>(i, cpuCycle);
        cores.push_back(core);
    }
}

void GlobalScheduler::runScheduler()
{
    // Start the current scheduler
    currentScheduler->start();
}

void GlobalScheduler::setScheduler(std::string schedulerAlgorithm, int quantumCycles)
{
    // Set the current scheduler based on the provided algorithm name
    auto it = schedulerAlgorithms.find(schedulerAlgorithm);
    if (it != schedulerAlgorithms.end())
    {
        currentScheduler = it->second;
        currentScheduler->setQuantumCycles(quantumCycles); // Set quantum cycles if applicable
        // std::cout << "Scheduler set to: " << schedulerAlgorithm << std::endl;
    }
    else
    {
        // std::cerr << "Scheduler algorithm not found: " << schedulerAlgorithm << std::endl;
    }
}

void GlobalScheduler::createProcess(std::string processName)
{
    auto process = processGenerator->createProcess(processName);
    processGenerator->assignToScheduler(process);
}

void GlobalScheduler::finishProcess(std::shared_ptr<Process> process)
{
    // Add the finished process to the list of finished processes
    std::lock_guard<std::mutex> lock(finishedProcessesMutex); // Lock the mutex to ensure thread safety
    finishedProcesses.push_back(process->getProcessName() + "    " +
                                process->getTimeFinished() + "    Finished    " +
                                std::to_string(process->getCurrentLine()) + " / " +
                                std::to_string(process->getTotalIntstruction()));
}

void GlobalScheduler::queueProcess(std::shared_ptr<Process> process)
{
    // Add a process to the scheduler's ready queue
    // Log locking for debugging
    // std::cout << "Locked queueProcess" << std::endl;
    std::lock_guard<std::mutex> lock(queueMutex); // Lock the mutex to ensure thread safety

    process->setState(READY);              // Set process state to READY
    currentScheduler->addProcess(process); // Add the process to the current scheduler's queue
}

void GlobalScheduler::addProcess(std::shared_ptr<Process> process)
{
    // Add a process to the process map
    // Log locking for debugging
    // std::cout << "Locked addProcess" << std::endl;
    std::lock_guard<std::mutex> lock(processMapMutex); // Lock the mutex to ensure thread safety
    processMap[process->getProcessName()] = process;
}

void GlobalScheduler::initializeProcessGeneration(int cpuCycle, int minInstructions, int maxInstructions)
{
    processGenerator = std::make_shared<SchedulerTestThread>(cpuCycle, minInstructions, maxInstructions);
}

void GlobalScheduler::startProcessGeneration()
{
    processGenerator->resume();
}

void GlobalScheduler::stopProcessGeneration()
{
    processGenerator->stop();
}

std::string GlobalScheduler::getCPUUtilization()
{
    int totalCores = static_cast<int>(cores.size());
    int usedCores = 0;

    for (const auto &core : cores)
    {
        if (core->isOccupied())
            usedCores++;
    }

    int utilization = (totalCores > 0) ? (usedCores * 100 / totalCores) : 0;
    return "CPU utilization: " + std::to_string(utilization) + "%";
}

std::string GlobalScheduler::getCoresUsed()
{
    int used = 0;
    for (const auto &core : cores)
    {
        if (core->isOccupied())
            used++;
    }
    return "Cores used: " + std::to_string(used);
}

std::string GlobalScheduler::getCoresAvailable()
{
    int used = 0;
    for (const auto &core : cores)
    {
        if (core->isOccupied())
            used++;
    }
    int total = static_cast<int>(cores.size());
    return "Cores available: " + std::to_string(total - used);
}

std::vector<std::string> GlobalScheduler::getRunningProcesses()
{
    // Get the list of running processes in string format
    // Return running processes in occupied cores
    std::vector<std::string> log;
    for (auto &core : this->cores)
    {
        if (core->isOccupied())
        {
            log.push_back(core->getProcess());
        }
    }

    return log;
}

std::vector<std::string> GlobalScheduler::getFinishedProcesses()
{
    // Return list of finished processes
    std::lock_guard<std::mutex> lock(finishedProcessesMutex); // Lock the mutex to ensure thread safety
    return this->finishedProcesses;
}

bool GlobalScheduler::processExists(const std::string &processName) const
{
    // Check if a process with the given name exists in the process map
    std::lock_guard<std::mutex> lock(processMapMutex); // Lock the mutex to ensure thread safety
    auto it = processMap.find(processName);
    if (it != processMap.end())
    {
        return true; // Process found
    }
    return false; // Process not found
}

std::shared_ptr<Process> GlobalScheduler::getProcessByName(const std::string &processName) const
{
    // Get a process by its name from the process map
    std::lock_guard<std::mutex> lock(processMapMutex); // Lock the mutex to ensure thread safety
    auto it = processMap.find(processName);
    if (it != processMap.end())
    {
        return it->second; // Return the found process
    }
    return nullptr; // Process not found
}

std::shared_ptr<Process> GlobalScheduler::getProcessByID(int processID) const
{
    // Get a process by its ID from the process map
    std::lock_guard<std::mutex> lock(processMapMutex); // Lock the mutex to ensure thread safety
    for (const auto &pair : processMap)
    {
        if (pair.second->getProcessID() == processID)
        {
            return pair.second; // Return the found process
        }
    }
    return nullptr; // Process not found
}
