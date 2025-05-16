#pragma once
#include <
#include "AConsole.h"

class ConsoleManager {
public:
    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    void drawConsole() const;
    void process() const;
    
    void exitApplication();
    bool isRunning() const;

    void setCursorPosition(int X, int Y) const;
    HANDLE getConsoleHandle() const;

private:
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance;

    HANDLE consoleHandle = nullptr;
    bool running;
}