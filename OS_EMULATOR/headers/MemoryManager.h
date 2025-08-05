#pragma once
#include <unordered_map>
#include <deque>
#include <vector>
#include <optional>
#include <iostream>
#include <list>
#include "Process.h"
#include "Memory.h"
#include "DiskManager.h"

class MemoryManager {
private:
    Memory *memory = nullptr;
    int pageSize;
    int maxFrames;

    struct FrameInfo {
        int processId;
        int virtualPage;
    };

    DiskManager* disk = DiskManager::getInstance();  // Swap space/backing store
    std::vector<std::optional<FrameInfo>> frameTable; // indexed by frame number

    // Replacement policy data
    std::deque<int> fifoQueue;             // for FIFO
    std::list<int> lruList;                // for LRU
    std::unordered_map<int, std::list<int>::iterator> lruMap;
    std::string policy; // "FIFO" or "LRU"

    int findFreeOrReplaceFrame();
    void updateLRU(int frameNumber);

    // Make singleton
    MemoryManager();
    MemoryManager(const MemoryManager &) = delete;
    MemoryManager &operator=(const MemoryManager &) = delete;
    static MemoryManager *sharedInstance;

public:
    static MemoryManager *getInstance();
    static void initialize();
    static void destroy();

    void setMemoryManager(int memorySize, int pageSize, int maxFrames);

    // ---- Page Handling ----
    int getFrame(std::shared_ptr<Process> proc, int virtualPage);
    int translate(std::shared_ptr<Process> proc, uint16_t vAddr);
   
    // ---- Read/Write ----
    uint16_t read16(std::shared_ptr<Process> proc, uint16_t vAddr);
    void write16(std::shared_ptr<Process> proc, uint16_t vAddr, uint16_t value);
    uint8_t fetchByte(std::shared_ptr<Process> proc, uint16_t vAddr);
    void writeByte(std::shared_ptr<Process> proc, uint16_t vAddr, uint8_t value);
};