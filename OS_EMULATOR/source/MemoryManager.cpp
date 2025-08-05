#include "MemoryManager.h"
#include "DiskManager.h"

MemoryManager *MemoryManager::sharedInstance = nullptr;

MemoryManager::MemoryManager() {
    this->disk = DiskManager::getInstance(); // Initialize here
    policy = "FIFO";
}

MemoryManager *MemoryManager::getInstance()
{
    return sharedInstance;
}

void MemoryManager::initialize()
{
    // Initialize the MemoryManager instance
    if (sharedInstance == nullptr)
    {
        sharedInstance = new MemoryManager();
    }
}

void MemoryManager::destroy()
{
    // Clean up the MemoryManager instance
    if (sharedInstance != nullptr)
    {
        sharedInstance = nullptr;
    }
}

void MemoryManager::setMemoryManager(int memorySize, int pageSize, int maxFrames) {
    this->memory = new Memory(memorySize);
    this->pageSize = pageSize;
    this->maxFrames = maxFrames;
    frameTable.reserve(maxFrames);
}

int MemoryManager::getMaxFrames() const {
    return maxFrames;
}

int MemoryManager::getPageSize() const
{
    return this->pageSize;
}

int MemoryManager::findFreeOrReplaceFrame() {
    // Get free frame index
    // std::cout << "Finding Free Frame" << std::endl;
    
    for (int i = 0; i < maxFrames; ++i) {
        if (!frameTable[i].has_value()) 
        {   
            return i;
        }

    }

    if (policy == "FIFO") {
        int victim = fifoQueue.front();
        fifoQueue.pop_front();
        return victim;
    } else { // LRU
        int victim = lruList.back();
        lruList.pop_back();
        lruMap.erase(victim);
        return victim;
    }
}

void MemoryManager::updateLRU(int frameNumber) {
    if (lruMap.count(frameNumber)) {
        lruList.erase(lruMap[frameNumber]);
    }
    lruList.push_front(frameNumber);
    lruMap[frameNumber] = lruList.begin();
}

 // ---- Page Handling ----

int MemoryManager::getFrame(std::shared_ptr<Process> proc, int virtualPage)
{
    // Get page in process
    // std::cout << "Recovering Page" << std::endl;
    Page& page = proc->pageTable[virtualPage];
    // std::cout << "Checking Page validation" << std::endl;
    // Check page if valid
    if (page.inMemory) {
        if (policy == "LRU") {
            updateLRU(page.frameNumber);
        }
        return page.frameNumber;
    }

    // ----- Page fault ------ //
    // std::cout << "[Page Fault!]" << std::endl;
    int frameNumber = findFreeOrReplaceFrame();
    // std::cout << "Frame " << frameNumber << " is free" << std::endl;
    // Eviction
    if (frameTable[frameNumber]) {
        // std::cout << "Evicting Page from Frame" << std::endl;
        // Evict page from frame
        const auto& evicted = frameTable[frameNumber].value();
        // Load page bytecode
        std::vector<uint8_t> bytecode;
        for (int i = 0; i < pageSize; ++i) {
            bytecode.push_back(memory->read(frameNumber * pageSize + i));
        }
        // Store page data into disk
        disk->storePage(evicted.processId, evicted.virtualPage, bytecode);
    }

    // std::cout << "Loading Page from Disk" << std::endl;
    // Load page data from disk if it exists
    std::vector<uint8_t> bytecode = disk->loadPage(proc->processID, pageSize, virtualPage);
    if (!bytecode.empty()) {
        for (int i = 0; i < pageSize; ++i) {
            memory->write(frameNumber * pageSize + i, bytecode[i]);
        }
    } else {
        // std::cout << "Page not found in Disk. Loading fresh page" << std::endl;
        // Fresh page, initialize to 0
        for (int i = 0; i < pageSize; ++i) {
            memory->write(frameNumber * pageSize + i, 0);
        }
    }

    // Update page info
    page.inMemory = true;
    page.frameNumber = frameNumber;
    page.referenced = true;
    page.dirty = false;
    proc->pageTable[virtualPage] = page;

    // Occupied frame added to the frameTable
    
    frameTable[frameNumber] = FrameInfo{proc->processID, virtualPage};

    //print frameTable
    // std::cout << "Frame: "<< frameTable[frameNumber].value().processId << ", " << frameTable[frameNumber].value().virtualPage << std::endl;
    

    if (policy == "FIFO") {
        fifoQueue.push_back(frameNumber);
    } else {
        lruList.push_front(frameNumber);
        lruMap[frameNumber] = lruList.begin();
    }

    return frameNumber;
}

// Translate a virtual address to a physical address in memory
int MemoryManager::translate(std::shared_ptr<Process> proc, uint16_t vAddr)
{
    // std::cout << "Translating Virtual Address: " << vAddr << "for process " << proc->name << std::endl;
    // std::cout << "PageSize: " << pageSize << std::endl;
    int vPage = static_cast<int>(vAddr) / pageSize;
    // std::cout << "vPage: " << vPage << std::endl;
    int offset = static_cast<int>(vAddr) % pageSize;
    // std::cout << "Offset: " << offset << std::endl;
    int frame = getFrame(proc, vPage);
    return frame * pageSize + offset;
}

// ---- Read/Write ----

uint16_t MemoryManager::read16(std::shared_ptr<Process> proc, uint16_t vAddr)
{
    int pAddr = translate(proc, vAddr);
    // std::cout << "Reading from Physical Address: " << pAddr << std::endl;
    int low = memory->read(pAddr);
    int high = memory->read(pAddr + 1);
    return static_cast<uint16_t>((high << 8) | low);
}

void MemoryManager::write16(std::shared_ptr<Process> proc, uint16_t vAddr, uint16_t value)
{
    int pAddr = translate(proc, vAddr);
    // std::cout << "Writing to Physical Address: " << pAddr << std::endl;
    memory->write(pAddr, value & 0xFF);
    memory->write(pAddr + 1, (value >> 8) & 0xFF);
}

uint8_t MemoryManager::fetchByte(std::shared_ptr<Process> proc, uint16_t vAddr)
{
    int pAddr = translate(proc, vAddr);
    return static_cast<uint8_t>(memory->read(pAddr));
}

void MemoryManager::writeByte(std::shared_ptr<Process> proc, uint16_t vAddr, uint8_t value)
{
    int pAddr = translate(proc, vAddr);
    memory->write(pAddr, value);
}

size_t MemoryManager::getMemorySize() const
{
    return memory->getMemorySize();
}

/*

Get the total memory usage by getting the number of all pages/processes in the frameTable
and multiplying by the page size.
This is a simple approximation and may not reflect the actual memory usage accurately


*/
size_t MemoryManager::getMemoryUsage() const
{
    size_t totalUsage = 0;
    for (int i = 0; i < maxFrames; ++i) {
        if (frameTable[i].has_value()) {    
            totalUsage++;
        }
    }
    // return totalUsage / (1024 * 1024); // Convert to MiB
    return (totalUsage * pageSize);

}

std::vector<std::string> MemoryManager::getFrameTable()
{
    std::vector<std::string> table;

    for (int i = 0; i < maxFrames; ++i) {
        std::cout<< frameTable[i].has_value() << std::endl;
        if (frameTable[i].has_value()) {
            table.push_back(std::to_string(frameTable[i].value().processId));
        }
    }

    return table;
}

