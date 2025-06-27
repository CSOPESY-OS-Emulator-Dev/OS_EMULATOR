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
    
    bool running = true;
    while(running) {
        // update the display
        ConsoleManager::getInstance()->draw();

        // handle the input
        ConsoleManager::getInstance()->process();

        running = ConsoleManager::getInstance()->isRunning();
    }
}
 