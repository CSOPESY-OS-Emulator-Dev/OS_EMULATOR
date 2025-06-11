#pragma once
#include <iostream>
#include "IETThread.h"

class SchedulerTestThread : public IETThread {
    // SchedulerTestThread is a test thread for the scheduler.
    // It inherits from IETThread to allow running in a separate thread.
    // This class is used to test the scheduler's functionality and performance.
    // This class creates new processes and adds them to the scheduler's queue.
    // Processes are created every cpu cycle.
public:
    SchedulerTestThread(int cpuCycle);
    void run() override;
    void stop();
    void resume();
private:
    int cpuCycle; // The CPU cycle for the test thread
    bool isRunning = true; // Flag to control the thread's execution loop
    int processCount = 0; // Counter for the number of processes created
};