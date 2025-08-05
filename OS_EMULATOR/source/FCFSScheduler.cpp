#include "FCFSScheduler.h"
#include "DiskManager.h"

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
                // 1. --- POP A PROCESS ---
                auto process = this->ReadyQueue.front();
                this->ReadyQueue.erase(this->ReadyQueue.begin());

                // 2. --- COMPILE PROCESS INSTRUCTIONS (ONCE) ---
                if (!process->hasBeenCompiled) {
                    std::vector<uint8_t> bytecode = compiler.compile(process->getInstructions());
                    
                    // Store compiler results in the process object for future use
                    process->stringLiterals = compiler.getStringLiterals();
                    process->setByteSize(bytecode.size());
                    process->hasBeenCompiled = true;

                    // 3. --- MEMORY VALIDATION ---
                    if (process->getByteSize() > process->getMemorySize()) {
                        // In a real OS, terminate it. Here, we just won't run it.
                        continue; // Skip to the next core
                    }
                    
                    // 4. --- STORE BYTECODE TO DISK ---
                    int pageSize = MemoryManager::getInstance()->getPageSize();
                    for (size_t i = 0; i < bytecode.size(); i += pageSize) {
                        int virtualPageNum = i / pageSize;
                        
                        // Create a vector for the current page's data
                        std::vector<uint8_t> pageData(
                            bytecode.begin() + i,
                            bytecode.begin() + std::min(i + pageSize, bytecode.size())
                        );
                        
                        DiskManager::getInstance()->storePage(process->getProcessID(), virtualPageNum, pageData);
                    }
                }
                
                // 5. --- ASSIGN TO CPU ---
                // The process is compiled and its program is on disk. Now assign it to the core.
                // The FCFS policy gives it enough ticks to run to completion.
                core->assignProcess(process, process->getTotalIntstruction());
            }
        }
    }
}