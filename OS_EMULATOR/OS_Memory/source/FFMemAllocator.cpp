#include "../header/FFMemAllocator.h"


FFMemAllocator::FFMemAllocator(size_t maxMemorySize) : maxMemorySize(maxMemorySize) {
    char* base = new char[maxMemorySize * sizeof(char)];
    baseAddress = base;
    memoryBlocks.emplace_back(std::make_shared<Block>(base, maxMemorySize, true)); // Initialize with one large free block
    blockMap[base] = memoryBlocks.back(); // Map the base address to the block
    // Optionally, initialize processID to -1 or some default value
    for (auto &block : memoryBlocks) {
        block->processID = -1; // Initialize processID to -1 indicating no process owns the block
    }
}

void *FFMemAllocator::allocate(Process &process)
{
    size_t size = process.getMemorySize();

    if (size == 0 || size > maxMemorySize) {
        return nullptr; // Invalid allocation request
    }
    void *ptr = findFreeBlock(size);
    if (ptr) {
        // Set the process ID for the allocated block
        blockMap[ptr]->processID = process.getProcessID();
        allocatedMemory++;
        remainingMemory = remainingMemory + size;
        return ptr;
    }
    // If no free block is found, return nullptr
    return nullptr;
}

void FFMemAllocator::deallocate(Process &process) {
    void *ptr = process.getBaseAddress();



    auto block = blockMap[ptr];
    // If the block is found and is currently allocated
    if (!block->isFree) {
        allocatedMemory--;
        remainingMemory = remainingMemory - block->size;
        block->isFree = true; // Mark the block as free
        block->processID = -1; // Reset the process ID
        mergeFreeBlocks(); // Attempt to merge adjacent free blocks
    }
   
}

std::string FFMemAllocator::visualizeMemory() const {
    std::string visualization;
    int blockAddress;
    for (const auto &block : memoryBlocks) {
        if (block->processID == -1) {
            continue; // Skip free blocks
        }
        blockAddress = reinterpret_cast<uintptr_t>(block->address) - reinterpret_cast<uintptr_t>(baseAddress);
        visualization = "\n" + std::to_string(blockAddress + block->size) + "\n"
                        + std::to_string(block->processID) + "\n"
                        + std::to_string(blockAddress) + "\n\n" + visualization;
    }
    return visualization;
}

void* FFMemAllocator::findFreeBlock(size_t size) {
    for (auto &block : memoryBlocks) {
        if (block->isFree && block->size >= size) {
            block->isFree = false; // Mark the block as used
            if (block->size > size) {
                splitBlock(block, size); // Split the block if it's larger than requested size
            }
            return block->address; // Return the address of the free block
        }
    }
    return nullptr; // No suitable free block found
}

void FFMemAllocator::splitBlock(std::shared_ptr<Block> block, size_t size) {
    void *newAddress = static_cast<char*>(block->address) + size;
    size_t remainingSize = block->size - size;
    block->size = size; // Resize the current block the requested size

    // Find the position of the current block
    auto it = std::find_if(memoryBlocks.begin(), memoryBlocks.end(),
        [&block](const std::shared_ptr<Block>& b) { return b->address == block->address; });

    auto newBlock = std::make_shared<Block>(newAddress, remainingSize, true);
    // Update the blockMap with the new block
    blockMap[newAddress] = newBlock;

    // Insert the new block right after the current one
    if (it != memoryBlocks.end() && remainingSize > 0) {
        memoryBlocks.insert(std::next(it), newBlock);
    } else if (remainingSize > 0) {
        memoryBlocks.emplace_back(newBlock);
    }
}

void FFMemAllocator::mergeFreeBlocks() {
    for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ) {
        auto next = std::next(it);
        if (next != memoryBlocks.end() && (*it)->isFree && (*next)->isFree) {
            // Merge the two adjacent free blocks
            (*it)->size += (*next)->size;
            blockMap.erase((*next)->address); // Remove from blockMap
            memoryBlocks.erase(next);
            // Do not advance 'it', check again in case of multiple adjacent free blocks
        } else {
            ++it;
        }
    }
}

//Remove when integrating to main header and source
std::string getFormattedCurrentTime()
{
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();
    auto in_time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to local time
    std::tm* localTime = std::localtime(&in_time_t); // Use localtime_s on Windows

    // Format the output
    std::ostringstream oss;
    oss << "(" << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << ms.count()
        << ' ' << std::put_time(localTime, "%p") << ")";
    return oss.str();
}

void FFMemAllocator::createSnapshot(int quantumCycle)
{
    std::string filename = "memory_stamp_" + std::to_string(quantumCycle) + ".txt";
    std::ofstream MyFile(filename);

    if (!MyFile.is_open())
    {
        std::perror("Error opening file");
        std::cerr << "Filename attempted: " << filename << std::endl;
        return;
    }

    MyFile <<"Timestamp: " << getFormattedCurrentTime() << "\n"; //Timestamp
    MyFile << "Number of processes in memory: " << allocatedMemory << "\n";
    MyFile << "Total External Fragmentation: " << maxMemorySize - remainingMemory << "\n";
    MyFile << "---------------------------------------------- = " << maxMemorySize << std::endl;
    MyFile << visualizeMemory();
    MyFile << "---------------------------------------------- = 0\n";


    MyFile.close();

    std::cout<<"Memory Block Size: " << blockMap.size() << std::endl;
}
