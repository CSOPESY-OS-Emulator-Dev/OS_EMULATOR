#include "../header/MemoryManager.h"
#include "../header/FFMemAllocator.h"

MemoryManager* MemoryManager::instance = nullptr;

MemoryManager::MemoryManager() {
    // Constructor is private or protected in the header file
}

MemoryManager* MemoryManager::getInstance() {
    if (instance == nullptr) {
        instance = new MemoryManager();
    }
    return instance;
}

void MemoryManager::initialize(size_t maxMemorySize) {
    this->allocator = std::make_unique<FFMemAllocator>(maxMemorySize);
}

void* MemoryManager::allocate(Process& process) {
    return this->allocator->allocate(process);
}

void MemoryManager::deallocate(Process& process) {
    this->allocator->deallocate(process);
}

void MemoryManager::createSnapshot(int quantumCycle) {
    if (allocator) {
        // allocator->createSnapshot(quantumCycle);
    } else {
        std::cerr << "[MemoryManager] Cannot create snapshot: Allocator not initialized.\n";
    }
    
}

std::string MemoryManager::visualizeMemory() const {
    return this->allocator->visualizeMemory();
}
