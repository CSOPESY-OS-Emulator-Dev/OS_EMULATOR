#include "ConsoleManager.h"

ConsoleManager::ConsoleManager() {
	this->running = true;

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	this->consoleTable["MAIN_CONSOLE"] = mainConsole;
	
	// Initialize MainConsole
	this->currentConsole = mainConsole;
}


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

void ConsoleManager::draw() const
{
	this->currentConsole->draw();
}

void ConsoleManager::process() const
{	
	std::string input;
	std::cout << "C:\\> ";
    std::getline(std::cin, input);
	this->currentConsole->process(input);
}

bool ConsoleManager::registerConsole(std::string consoleName)
{
	auto scheduler = GlobalScheduler::getInstance();

    if (!scheduler->processExists(consoleName)) {
        scheduler->createProcess(consoleName); 
    }
    if (consoleTable.find(consoleName) == consoleTable.end()) {
        consoleTable[consoleName] = std::make_shared<ProcessConsole>(consoleName, getFormattedCurrentTime());
    }
	if (scheduler->getProcessByName(consoleName)->getState() != FINISHED) {
		switchConsole(consoleName);
		return true;
	}
	return false;
}

bool ConsoleManager::switchConsole(std::string consoleName)
{
	auto scheduler = GlobalScheduler::getInstance();
	if (consoleTable.find(consoleName) == consoleTable.end()) {
		return false;
	}
	if (scheduler->getProcessByName(consoleName)->getState() == FINISHED) {
		return false;
	}
	std::cout << std::flush;
    std::cout << "\033c";
	this->previousConsole = this->currentConsole;
	this->currentConsole = this->consoleTable[consoleName];
	this->currentConsole->initialize();
	return true;
}

void ConsoleManager::returnToPreviousConsole()
{
	this->currentConsole = this->previousConsole;
	this->previousConsole = nullptr;
	this->currentConsole->initialize();
}

bool ConsoleManager::isRunning() const
{
    return this->running;
}

void ConsoleManager::exitApplication()
{
    this->running = false;
}

// Temporary function

std::string ConsoleManager::getFormattedCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);  // For Windows use localtime_s

    std::ostringstream oss;
    oss << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}
