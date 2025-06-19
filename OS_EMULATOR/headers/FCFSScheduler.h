#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "CoreThread.h"

class FCFSScheduler : public AScheduler {
    // FCFSScheduler implements the First-Come, First-Served scheduling algorithm.
    // It inherits from AScheduler and overrides the execute method to implement the FCFS logic.
public:
    FCFSScheduler();
    ~FCFSScheduler();

    // Override the execute method to implement FCFS scheduling logic
    void execute() override;
};