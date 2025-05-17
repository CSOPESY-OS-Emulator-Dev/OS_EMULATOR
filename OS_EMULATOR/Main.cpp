// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* CSOPESY DEVELOPERS:
 * DEMANALATA, ASHANTIE LOUIZE BACO
 * GONZALES, BRYANT KIMM CUSTODIO
 * KINTANAR, KRISTIAN ANTHONY REMEDIOS
 * OLORES, SEAN ANDREI PAJARTIN
 */

#include "ConsoleManager.h"

int main()
{
    ConsoleManager::initialize();

    bool running = true;
    while(running) {
        ConsoleManager::getInstance()->draw();
        ConsoleManager::getInstance()->process();

        running = ConsoleManager::getInstance()->isRunning();
    }
}
