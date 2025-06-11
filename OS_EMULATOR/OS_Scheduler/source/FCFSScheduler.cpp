#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler() {
    // Constructor implementation
}
FCFSScheduler::~FCFSScheduler() {
    // Destructor implementation
}

void FCFSScheduler::execute() {
    // This method implements the Multicore, First-Come, First-Served scheduling logic.
    if (!this->ReadyQueue.empty()) {
        // Find an available core to assign the process
        for (const auto& core : GlobalScheduler::getInstance()->cores) {
            if (this->ReadyQueue.empty()) break; // If the queue is empty, break the loop
            auto process = this->ReadyQueue.front(); // Get the first process in the queue
            if (!core->isOccupied()) {
                core->assignProcess(process, process->getTotalIntstruction() + 10); // Assign process with its total instructions as ticks
                this->ReadyQueue.erase(this->ReadyQueue.begin()); // Remove it from the queue
            }
        }
    }
}