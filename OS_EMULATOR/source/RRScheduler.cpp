#include "RRScheduler.h"

RRScheduler::RRScheduler() {
    // Constructor implementation
}

RRScheduler::~RRScheduler() {
    // Destructor implementation
}

void RRScheduler::execute() {
    std::lock_guard<std::mutex> lock(queueMutex); // Lock the queue to prevent race conditions

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

                // for debugging purposes, log the assignment
                std::cout << "[RR] Assigning Process " 
                          << process->getProcessName()
                          << " to Core " << core->getCoreID()
                          << " with quantum " << this->quantumCycles << std::endl;


                // Assign the process to the core with quantumCycles ticks
                core->assignProcess(process, this->quantumCycles);

                // If the process is not finished, add it back to the end of the queue later
                // (This is handled inside CoreThread automatically by checking ticks and re-queuing)
            }
        }
    }
}