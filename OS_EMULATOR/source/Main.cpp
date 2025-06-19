// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* CSOPESY DEVELOPERS:
 * DEMANALATA, ASHANTIE LOUIZE BACO
 * HILOMEN, GEO BRIAN
 * KINTANAR, KRISTIAN ANTHONY REMEDIOS
 * OLORES, SEAN ANDREI PAJARTIN
 */

#include "ConsoleManager.h"
#include "GlobalScheduler.h"

int main()
{
    // initialize console and scheduler
    ConsoleManager::initialize();
    GlobalScheduler::initialize();

    // set up cores and scheduler
    GlobalScheduler::getInstance()->initializeCores(4, 4);
    GlobalScheduler::getInstance()->runCores();
    GlobalScheduler::getInstance()->setScheduler("fcfs", 4); 
    // GlobalScheduler::getInstance()->setScheduler("rr"); 

    // Initialize process generation
    GlobalScheduler::getInstance()->runScheduler();
    
    // Note: in the future, verify correct configs (no negative or minimum > max)
    GlobalScheduler::getInstance()->initializeProcessGeneration(1, 100, 100); // para always 100

    // Create 10 processes ("process0 - process9")
    for(int i = 0; i < 10; i++) {
        std::string name = "process" + std::to_string(i);
        GlobalScheduler::getInstance()->createProcess(name);
    }

    bool running = true;
    while(running) {
        // update the display
        ConsoleManager::getInstance()->draw();

        // handle the input
        ConsoleManager::getInstance()->process();

        running = ConsoleManager::getInstance()->isRunning();
    }
}
 