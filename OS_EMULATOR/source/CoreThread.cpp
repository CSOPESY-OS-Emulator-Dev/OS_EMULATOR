#include "CoreThread.h"

CoreThread::CoreThread(int id, int cpuCycle) : coreID(id), cpuCycle(cpuCycle), cpuTicks(0), activeTicks(0), currentTicks(0) {
    this->currentProcess = nullptr; // Initialize current process to nullptr
    this->occupied = false; // Initially, the core is not occupied
}

void CoreThread::run() {
    // This method runs in a loop, simulating the core's operation.
    // A delay is introduced using a cpu cycle to control the execution speed.
    while (this->isRunning) {
        // If the core is occupied and the current process is set, check if it can execute
        if (this->occupied && this->currentProcess && this->cpuTicks % (this->cpuCycle + 1) == 0) {
            // Check if the process has finished executing
            if (this->currentProcess->getState() == WAITING) {
                // Queue current process back to scheduler in GlobalScheduler
                GlobalScheduler::getInstance()->queueProcess(this->currentProcess);
                this->occupied = false; // Free the core
                this->currentProcess = nullptr; // Clear the current process
                this->currentTicks = 0; // Reset current ticks
            } else if (this->currentProcess->getState() == FINISHED) {
                // Push current process to finished processes in GlobalScheduler
                GlobalScheduler::getInstance()->finishProcess(this->currentProcess);
                this->occupied = false; // Free the core
                this->currentProcess = nullptr; // Clear the current process
                this->currentTicks = 0; // Reset current ticks
            } else if (this->currentTicks <= 0) {
                // Queue current process back to scheduler in GlobalScheduler
                GlobalScheduler::getInstance()->queueProcess(this->currentProcess);
                this->occupied = false; // Free the core
                this->currentProcess = nullptr; // Clear the current process
                this->currentTicks = 0; // Reset current ticks
            } else if (this->currentProcess->getState() == SLEEPING) {
                // If process is currently executing a sleep instruction
                this->currentProcess->executeInstruction();
            } else {
                // If the process is still running and has ticks left, execute its instruction
                this->currentProcess->executeInstruction(); // Execute the current process's instruction
                this->currentTicks--; // Decrease the ticks for the current process
            } 
        }
        this->cpuTicks++;
        sleep(1); // Sleep for 10 millisecond to simulate time passing
    }
}

void CoreThread::stop() {
    this->isRunning = false; // Stop the core thread
    // std::cout << "Core " << this->coreID << " stopped." << std::endl; // Log the stop action
}

void CoreThread::assignProcess(std::shared_ptr<Process> process, int ticks) {
    this->currentProcess = process;
    this->currentProcess->setCoreID(this->coreID); // Set the core ID for the process
    this->currentProcess->setState(RUNNING); // Set the process state to RUNNING
    
    this->activeTicks += ticks;
    this->currentTicks = ticks;
    this->occupied = true;
    
    // std::cout << "Process " << this->currentProcess->getProcessID() << " assigned to Core " << this->coreID << " with " << ticks << " ticks." << std::endl;
}

bool CoreThread::isOccupied() const {
    return this->occupied; // Return whether the core is currently occupied
}

std::string CoreThread::getProcess() {
    // Return a string representation of the current process running on this core
    return currentProcess->getProcessName() + "    " + 
           currentProcess->getTimeRunning() + "    Core: " + 
           std::to_string(this->coreID) + "    " + 
           std::to_string(currentProcess->getCurrentLine()) + " / " + 
           std::to_string(currentProcess->getTotalIntstruction());
}