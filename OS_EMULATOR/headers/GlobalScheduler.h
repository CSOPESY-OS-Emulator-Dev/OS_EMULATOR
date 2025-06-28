#pragma once
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>
#include "Process.h"
#include "AScheduler.h"
#include <string>
#include <sstream>

class CoreThread;
class SchedulerTestThread;
class FCFSScheduler;

class GlobalScheduler
{
public:
    static GlobalScheduler *getInstance();
    static void initialize();
    static void destroy();

    void runCores();
    void initializeCores(int numCores, int cpuCycle);
    void runScheduler();
    void setScheduler(std::string schedulerAlgorithm, int quantumCycles);

    // Create a new process
    void createProcess(std::string processName);
    // Add finished process to the list
    void finishProcess(std::shared_ptr<Process> process);
    // Add a process to the scheduler's readyqueue
    void queueProcess(std::shared_ptr<Process> process);
    // Add a process to the process map
    void addProcess(std::shared_ptr<Process> process);

    // Initialize thread for process generation
    void initializeProcessGeneration(int cpuCycle, int minInstructions, int maxInstructions);
    // Start thread for generating processes
    void startProcessGeneration();
    // Stop thread for generating processes
    void stopProcessGeneration();

    // Get the CPU details in string format
    std::string getCPUUtilization();
    std::string getCoresUsed();
    std::string getCoresAvailable();

    // Get the list of running processes in string
    std::vector<std::string> getRunningProcesses();
    // Get the list of finished processes in string
    std::vector<std::string> getFinishedProcesses();

    // Check if a process with the given name exists
    bool processExists(const std::string &processName) const;
    // Get a process by its name
    std::shared_ptr<Process> getProcessByName(const std::string &processName) const;
    // Get a process by its ID
    std::shared_ptr<Process> getProcessByID(int processID) const;

    friend class FCFSScheduler; // Allow FCFS Scheduler to access private members
private:
    // Make singleton
    GlobalScheduler();
    GlobalScheduler(const GlobalScheduler &) = delete;
    GlobalScheduler &operator=(const GlobalScheduler &) = delete;
    static GlobalScheduler *sharedInstance;

    // List of cores in the system
    std::vector<std::shared_ptr<CoreThread>> cores;
    // Current scheduler algorithm
    std::shared_ptr<AScheduler> currentScheduler;
    // Scheduler Test thread
    std::shared_ptr<SchedulerTestThread> processGenerator;
    // Map of available scheduler algorithms
    std::unordered_map<std::string, std::shared_ptr<AScheduler>> schedulerAlgorithms;
    // Map of processes by PID
    std::unordered_map<std::string, std::shared_ptr<Process>> processMap;
    // List of finished processes
    std::vector<std::string> finishedProcesses;

    // Mutex to ensure thread safety when accessing shared resources
    mutable std::mutex coreMutex;              // Mutex to ensure thread safety when accessing core threads
    mutable std::mutex schedulerMutex;         // Mutex to ensure thread safety when accessing scheduler resources
    mutable std::mutex processMapMutex;        // Mutex to ensure thread safety when accessing processMap
    mutable std::mutex finishedProcessesMutex; // Mutex to ensure thread safety when accessing finishedProcesses
    mutable std::mutex queueMutex;             // Mutex to ensure thread safety when accessing the ready queue
};