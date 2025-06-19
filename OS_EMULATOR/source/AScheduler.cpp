#include "AScheduler.h"

AScheduler::AScheduler() {
    // Constructor implementation
}

AScheduler::~AScheduler() {
    // Destructor implementation
}

void AScheduler::run() {
    while (this->isRunning) {
        // This method runs in a loop, simulating the scheduler's operation.
        // It can be overridden by derived classes to implement specific scheduling logic.
        // The execute method should contain the scheduling logic for the specific algorithm.
        execute();
        IETThread::sleep(10); // Sleep for a while to simulate scheduling delay
    }
}

void AScheduler::stop() {
    // Stop the scheduler thread
    isRunning = false; // Set the running flag to false
    // std::cout << "Scheduler stopped." << std::endl;
}

void AScheduler::addProcess(std::shared_ptr<Process> process) {
    // Locks the queue using mutex to ensure thread safety when adding a process
    // Log locking for debugging
    // std::cout << "Locked addProcess in queue" << std::endl;
    std::lock_guard<std::mutex> lock(queueMutex); // Lock the queue to ensure thread safety
    this->ReadyQueue.push_back(process); // Add the process to the ready queue
}

void AScheduler::setQuantumCycles(int cycles) {
    // Set the quantum cycles for time-slicing in round robin preemptive scheduling
    this->quantumCycles = cycles;
    // std::cout << "Quantum cycles set to: " << cycles << std::endl;
}
