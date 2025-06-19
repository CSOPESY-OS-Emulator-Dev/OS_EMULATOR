#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include "IETThread.h"
#include "Process.h"
#include "GlobalScheduler.h"

class CoreThread : public IETThread {
    // CoreThread represents a single CPU core in the system.
    // It manages the execution of processes assigned to it, handling their states and ticks.
    // It inherits from IETThread to allow running in a separate thread.
    // It contains methods to assign processes, check occupancy, and manage the core's execution cycle.
    // It also keeps track of the core's ID, CPU cycle, and ticks for process execution.
    // A core will run continuously until it is stopped.
public:
    CoreThread(int id, int cpuCycle);    
    void run() override;
    void stop();

    void assignProcess(std::shared_ptr<Process> process, int ticks);
    bool isOccupied() const;

    std::string getProcess();
    
private:
    std::shared_ptr<Process> currentProcess; // The process currently assigned to this core
    bool occupied = false; // Indicates if the core is currently occupied by a process
    bool isRunning = true; // Indicates if the core thread is running

    // Core properties
    int coreID;
    int cpuCycle;
    int cpuTicks;
    int activeTicks;
    int currentTicks;
};