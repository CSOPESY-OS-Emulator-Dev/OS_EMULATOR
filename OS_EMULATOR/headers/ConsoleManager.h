#pragma once
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <ctime>
#include "MainConsole.h"
#include "ProcessConsole.h"

class ConsoleManager {
public:
    typedef std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;

    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    void draw() const;
    void process() const;
    bool switchConsole(std::string consoleName);
    bool registerConsole(std::string consoleName);
    void returnToPreviousConsole();

    void setCursorPosition(int X, int Y) const;

    // Temporary Function
    std::string getFormattedCurrentTime();
    
    void exitApplication();
    bool isRunning() const;

private:
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance;

    ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

    bool running;
};