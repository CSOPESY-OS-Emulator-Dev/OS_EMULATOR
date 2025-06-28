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
            if (this->currentProcess->getState() == FINISHED) {

                // for debugging purposes, log the process completion
                std::cout << "[Core " << this->coreID << "] "
                          << this->currentProcess->getProcessName()
                          << " finished execution." << std::endl;


                // Push current process to finished processes in GlobalScheduler
                GlobalScheduler::getInstance()->finishProcess(this->currentProcess);
                this->occupied = false; // Free the core
                this->currentProcess = nullptr; // Clear the current process
                this->currentTicks = 0; // Reset current ticks
            } else if (this->currentTicks <= 0) {

                // for debugging purposes, log the quantum expiration
                std::cout << "[Core " << this->coreID << "] Quantum expired for "
                          << this->currentProcess->getProcessName()
                          << ". Requeuing." << std::endl;


                // Queue current process back to scheduler in GlobalScheduler
                GlobalScheduler::getInstance()->queueProcess(this->currentProcess);
                this->occupied = false; // Free the core
                this->currentProcess = nullptr; // Clear the current process
                this->currentTicks = 0; // Reset current ticks
            } else {
                // If the process is still running and has ticks left, execute its instruction
                this->currentProcess->executeInstruction(); // Execute the current process's instruction
                this->currentTicks--; // Decrease the ticks for the current process
                // log process execution for debugging
                // std::cout << "Core " << this->coreID 
                //           << " executed instruction for Process " 
                //           << this->currentProcess->getProcessID() 
                //           << " at CPU Tick: "
                //           << this->cpuTicks
                //           << ". Remaining ticks: " << this->currentTicks 
                //           << std::endl;

                // for debugging purposes, log the instruction execution
                std::cout << "[Core " << this->coreID << "] Executed instruction for "
                          << this->currentProcess->getProcessName()
                          << ", ticks left: " << this->currentTicks << std::endl;
            } 
        }
        this->cpuTicks++;
        sleep(10); // Sleep for 10 millisecond to simulate time passing
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