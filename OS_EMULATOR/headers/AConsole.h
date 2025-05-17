#pragma once
#include <iostream>
#include <sstream>
#include <vector>


struct ParsedCommand {
    std::string command;
    std::vector<std::string> args;
};

class AConsole {
public:
    AConsole(std::string name);
    ~AConsole() = default;

    virtual void draw() = 0;
    virtual void process(std::string input) = 0;
    virtual void initialize() = 0;

    ParsedCommand parseInput(const std::string& input);
private:
    std::string name;
};