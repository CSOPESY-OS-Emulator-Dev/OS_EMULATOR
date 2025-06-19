#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include "IETThread.h"
#include "Process.h"

class AScheduler : public IETThread {
    // AScheduler is an abstract class that defines the interface for all schedulers.
    // It inherits from IETThread to allow scheduling in a separate thread.
    // This class will be extended by specific scheduler implementations.
public:
    AScheduler();
    ~AScheduler();
    // Start the scheduler thread
    void run() override;
    // Stop the scheduler thread
    void stop();
    // Add a process to the scheduler's queue
    void addProcess(std::shared_ptr<Process> process);

protected:
    // Queue of processes to be scheduled
    std::vector<std::shared_ptr<Process>> ReadyQueue;
    // This method should be overridden by derived classes to implement specific scheduling logic
    virtual void execute() = 0;
    // Mutex to ensure thread safety when accessing the process queue
    mutable std::mutex queueMutex; 
private:
    bool isRunning = true; // Flag to control the scheduler's execution loop
};