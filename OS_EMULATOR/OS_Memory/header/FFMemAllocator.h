#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <memory>
#include <list>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "IMemoryAllocator.h"

class FFMemAllocator : public IMemoryAllocator {
public:
    FFMemAllocator(size_t maxMemorySize);

    void* allocate(Process &process) override;
    void deallocate(Process &process) override;
    std::string visualizeMemory() const override;
    void createSnapshot(int quantumCycle);

private:
    struct Block {
        void* address;
        size_t size;
        bool isFree;
        int processID = -1; // Optional: to track which process owns the block

        // Default constructor
        Block() : address(nullptr), size(0), isFree(false) {}
        Block(void* addr, size_t sz, bool free) : address(addr), size(sz), isFree(free) {}
    };

    size_t maxMemorySize;
    std::list<std::shared_ptr<Block>> memoryBlocks;
    std::unordered_map<void*, std::shared_ptr<Block>> blockMap;
    void* baseAddress;

    void* findFreeBlock(size_t size);
    void splitBlock(std::shared_ptr<Block>, size_t size);
    void mergeFreeBlocks();
};