#pragma once
#include <unordered_map>
#include <iostream>
#include <memory>
#include <vector>
#include "FCFSScheduler.h"

class CoreThread;

class GlobalScheduler {
public:
    static GlobalScheduler* getInstance();
    static void initialize();
    static void destroy();

    void runCores();
    void initializeCores(int numCores, int cpuCycle);
    void runScheduler();
    void setScheduler(std::string schedulerAlgorithm);

    // Add finished process to the list
    void finishProcess(std::shared_ptr<Process> process);
    // Add a process to the scheduler's readyqueue
    void addProcess(std::shared_ptr<Process> process);

    // Start thread for generating processes
    void startProcessGeneration();
    // Stop thread for generating processes
    void stopProcessGeneration();

    // Get the list of finished processes
    const std::vector<std::shared_ptr<Process>>& getFinishedProcesses() const;
    friend class FCFSScheduler; // Allow FCFS Scheduler to access private members
private:
    // Make singleton
    GlobalScheduler();
    GlobalScheduler(const GlobalScheduler&) = delete;
    GlobalScheduler& operator=(const GlobalScheduler&) = delete;
    static GlobalScheduler* sharedInstance;

    // List of cores in the system
    std::vector<std::shared_ptr<CoreThread>> cores;
    // Current scheduler algorithm
    std::shared_ptr<AScheduler> currentScheduler;
    // Map of available scheduler algorithms
    std::unordered_map<std::string, std::shared_ptr<AScheduler>> schedulerAlgorithms;
    // Map of processes by PID
    std::unordered_map<int, std::shared_ptr<Process>> processes;
    // List of finished processes
    std::vector<std::shared_ptr<Process>> finishedProcesses;

    // Mutex to ensure thread safety when accessing shared resources
    mutable std::mutex schedulerMutex; // Mutex to ensure thread safety when accessing scheduler resources
};