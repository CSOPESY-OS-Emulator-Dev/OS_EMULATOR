#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
    return sharedInstance;
}

void ConsoleManager::initialize()
{
    if (!sharedInstance) {
		sharedInstance = new ConsoleManager();
	}
}

void ConsoleManager::destroy()
{
    delete sharedInstance;  
	sharedInstance = nullptr;
}

void ConsoleManager::drawConsole() const
{
}

void ConsoleManager::process() const
{
}


bool ConsoleManager::isRunning() const
{
    return this->running;
}

void ConsoleManager::setCursorPosition(int X, int Y) const
{
    COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(this->getConsoleHandle(), coord);
}

HANDLE ConsoleManager::getConsoleHandle() const {	
	return consoleHandle;
}

void ConsoleManager::exitApplication()
{
    this->running = false;
}


