#pragma once

#include <memory>
#include <string>
#include "IMemoryAllocator.h"
#include "Process.h"

class MemoryManager {
public:
    // Get the singleton instance of MemoryManager
    static MemoryManager* getInstance();

    // Initialize memory settings (e.g., total memory size)
    void initialize(size_t maxMemorySize);

    // Allocate memory for a process
    void* allocate(Process& process);

    // Deallocate memory for a process
    void deallocate(Process& process);

    // Snapshot current memory state (called every quantum cycle)
    void createSnapshot(int quantumCycle);

    // View current memory state as a string (optional)
    std::string visualizeMemory() const;

private:
    // Private constructor for singleton
    MemoryManager();

    // Static instance
    static MemoryManager* instance;

    // Allocator (using First-Fit)
    std::unique_ptr<IMemoryAllocator> allocator;

    // Store max memory size for snapshots or stats
    size_t maxMemorySize;
};
