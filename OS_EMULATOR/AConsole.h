#pragma once
#include <iostream>

class AConsole {
public:
    AConsole(std::string name);
    ~AConsole() = default;

    virtual void draw() = 0;
    virtual void process() = 0;
    virtual void initializeConsole() = 0;

private:
    std::string name;
}