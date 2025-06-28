#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler() {
    // Constructor implementation
}
FCFSScheduler::~FCFSScheduler() {
    // Destructor implementation
}

void FCFSScheduler::execute() {
    // This method implements the Multicore, First-Come, First-Served scheduling logic.
    std::lock_guard<std::mutex> lock(queueMutex); // Add this lock
    if (!this->ReadyQueue.empty()) {
        // Find an available core to assign the process
        for (const auto& core : GlobalScheduler::getInstance()->cores) {
            if (this->ReadyQueue.empty()) break; // If the queue is empty, break the loop
            if (!core->isOccupied()) {
                auto process = this->ReadyQueue.front(); // Get the first process in the queue
                core->assignProcess(process, process->getTotalIntstruction()); // Assign process with its total instructions as ticks
                this->ReadyQueue.erase(this->ReadyQueue.begin()); // Remove it from the queue
            }
        }
    }
}