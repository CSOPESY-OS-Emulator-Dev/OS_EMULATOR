#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "AScheduler.h"
#include "GlobalScheduler.h"
#include "CoreThread.h"

#include "MemoryManager.h"

class RRScheduler : public AScheduler {
    // RRScheduler implements the Round Robin scheduling algorithm.
    // It inherits from AScheduler and overrides the execute method to implement the RR logic.
public:
    RRScheduler();
    ~RRScheduler();

    // Override the execute method to implement RR scheduling logic
    void execute() override;
};