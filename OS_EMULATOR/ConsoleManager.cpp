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

void ConsoleManager::switchConsole(std::string consoleName)
{
	if (this->consoleTable.find(consoleName) != this->consoleTable.end()) {
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->initialize();
	} else {
		std::cout << "Console " << consoleName << " does not exist." << std::endl;
	}
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