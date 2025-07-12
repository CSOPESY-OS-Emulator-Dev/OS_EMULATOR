#include <iostream>
#include <vector>
#include <string>
#include "FFMemAllocator.h"
#include "Process.h"

int main() {
    
    // Example usage of multiple Processes and FFMemAllocator classes
    FFMemAllocator allocator(16384); // Create a memory allocator with 1024 bytes
    Process process1("Process1", 1, 10, 4096); // Create a process with 256 bytes memory
    Process process2("Process2", 2, 20, 4096); // Create another process with 128 bytes memory
    Process process3("Process3", 3, 30, 4096); // Create a third process with 512 bytes memory
    Process process4("Process4", 4, 40, 4096); // Create a fourth process with 64 bytes memory
    Process process5("Process5", 5, 50, 4096); // Create a fifth process with 32 bytes memory
    Process process6("Process6", 6, 60, 4096); // Create a sixth process with 16 bytes memory
    Process process7("Process7", 7, 70, 4096); // Create a seventh process with 8 bytes memory
    Process process8("Process8", 8, 80, 4096); // Create an eighth process with 4 bytes memory
    Process process9("Process9", 9, 90, 4096); // Create a ninth process with 2 bytes memory
    Process process10("Process10", 10, 100, 4096); // Create a tenth process with 1 byte memory

    allocator.createSnapshot(6);


    // Allocate memory for each process
    void* addr1 = allocator.allocate(process1);
    process1.setBaseAddress(addr1); // Set the base address in the process
    void* addr2 = allocator.allocate(process2);
    process2.setBaseAddress(addr2); // Set the base address in the process
    void* addr3 = allocator.allocate(process3);
    process3.setBaseAddress(addr3); // Set the base address in the process
    void* addr4 = allocator.allocate(process4);
    process4.setBaseAddress(addr4); // Set the base address in the process
    void* addr5 = allocator.allocate(process5);
    process5.setBaseAddress(addr5); // Set the base address in the process

    // Visualize memory allocation
    std::cout << "Memory Visualization:\n" << allocator.visualizeMemory() << std::endl;
    // Deallocate memory for some processes
    allocator.deallocate(process1);
    allocator.deallocate(process2);
    allocator.deallocate(process3);

    // Visualize memory after deallocation
    std::cout << "Memory Visualization after deallocation:\n" << allocator.visualizeMemory() << std::endl;

    // Allocate memory for remaining processes
    void* addr6 = allocator.allocate(process6);
    void* addr7 = allocator.allocate(process7);
    void* addr8 = allocator.allocate(process8);
    void* addr9 = allocator.allocate(process9);
    void* addr10 = allocator.allocate(process10);

    // Visualize memory after allocating remaining processes
    std::cout << "Memory Visualization after allocating remaining processes:\n" << allocator.visualizeMemory() << std::endl;

    // Deallocate all remaining processes
    allocator.deallocate(process6);
    allocator.deallocate(process7);
    allocator.deallocate(process8);
    allocator.deallocate(process9);
    allocator.deallocate(process10);

    // Final memory visualization
    std::cout << "Final Memory Visualization:\n" << allocator.visualizeMemory() << std::endl;



    return 0;
}