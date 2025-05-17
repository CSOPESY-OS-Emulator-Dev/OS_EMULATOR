#pragma once
#include <unordered_map>
#include <memory>
#include "MainConsole.h"

class ConsoleManager {
public:
    typedef std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;

    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    void draw() const;
    void process() const;
    void switchConsole(std::string consoleName);
    void returnToPreviousConsole();

    void setCursorPosition(int X, int Y) const;
    
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