#include <iostream>
#include <string>
#include "../header/MemoryManager.h"
#include "../header/Process.h"

int main() {
    // Step 1: Initialize the MemoryManager singleton with 16 KB
    MemoryManager::getInstance()->initialize(16384);  // 16 KB total memory

    // Step 2: Create processes (each asking for 4096 bytes)
    Process process1("Process1", 1, 10, 4096);
    Process process2("Process2", 2, 20, 4096);
    Process process3("Process3", 3, 30, 4096);
    Process process4("Process4", 4, 40, 4096);
    Process process5("Process5", 5, 50, 4096);

    // Step 3: Allocate memory for processes
    process1.setBaseAddress(MemoryManager::getInstance()->allocate(process1));
    process2.setBaseAddress(MemoryManager::getInstance()->allocate(process2));
    process3.setBaseAddress(MemoryManager::getInstance()->allocate(process3));

    // Step 4: Create snapshot after first allocations
    MemoryManager::getInstance()->createSnapshot(1);
    std::cout << "📸 Snapshot 1 saved: memory_stamp_1.txt\n";

    // Step 5: Deallocate one process
    MemoryManager::getInstance()->deallocate(process2);

    // Step 6: Snapshot after deallocation
    MemoryManager::getInstance()->createSnapshot(2);
    std::cout << "📸 Snapshot 2 saved: memory_stamp_2.txt\n";

    // Step 7: Allocate more
    process4.setBaseAddress(MemoryManager::getInstance()->allocate(process4));
    process5.setBaseAddress(MemoryManager::getInstance()->allocate(process5));

    // Step 8: Snapshot again
    MemoryManager::getInstance()->createSnapshot(3);
    std::cout << "📸 Snapshot 3 saved: memory_stamp_3.txt\n";

    // Final: Visualize memory
    std::cout << "\n🧠 Final Memory Visualization:\n";
    std::cout << MemoryManager::getInstance()->visualizeMemory() << std::endl;

    return 0;
}
