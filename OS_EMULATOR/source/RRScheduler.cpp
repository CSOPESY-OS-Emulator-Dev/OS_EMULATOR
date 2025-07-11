#include "RRScheduler.h"

RRScheduler::RRScheduler() {
    // Constructor implementation
}

RRScheduler::~RRScheduler() {
    // Destructor implementation
}

void RRScheduler::execute() {
    std::lock_guard<std::mutex> lock(queueMutex); // Add this lock
    if (!this->ReadyQueue.empty()) {
        // Loop through all CPU cores
        for (const auto& core : GlobalScheduler::getInstance()->cores) {
            // If there are no more processes to schedule, stop
            if (this->ReadyQueue.empty()) break;
            // If the core is free, assign a process
            if (!core->isOccupied()) {
                // Get the process at the front of the queue
                auto process = this->ReadyQueue.front();
                this->ReadyQueue.erase(this->ReadyQueue.begin()); // Remove it from the queue
                core->assignProcess(process, this->quantumCycles); // Assign the process to the core with quantumCycles ticks
                // If the process is not finished, add it back to the end of the queue later
                // (This is handled inside CoreThread automatically by checking ticks and re-queuing)
            }
        }
    }
}