#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <variant>
#include <unordered_map>
#include <regex>
#include <cctype> // Required for isspace

enum class OpCode : uint8_t {
    PRINT = 0x01, // 0x01 = PRINT(msg.id)
                  // 0x11 = PRINT(msg.id, var.id)
    DECLARE	= 0x02,
    ADD	= 0x03, // 0x03 = ADD(var.id, var.id, var.id)
                // 0x13 = ADD(var.id, value, var.id)
                // 0x23 = ADD(var.id, var.id, value)
                // 0x33 = ADD(var.id, value, value)
    SUBTRACT = 0x04, // 0x04 = SUBTRACT(var.id, var.id, var.id)
                     // 0x14 = SUBTRACT(var.id, value, var.id)
                     // 0x24 = SUBTRACT(var.id, var.id, value)
                     // 0x34 = SUBTRACT(var.id, value, value)
    WRITE = 0x05, // 0x05 = WRITE(address, value)
    READ = 0x06, // 0x06 = READ(var.id, address)
    NOP = 0x07, // Skip instsruction NOP, NOP, NOP, ... , X
    SLEEP = 0x08, // 0x08 = SLEEP(X)
    JMP_ZERO = 0x09, // 0x09 = JMP_ZERO(reg.id, address)
    JMP = 0x19, // 0x19 = JMP(address)
    MOV = 0x0A, // 0x0A = MOV(reg.id, value)
    HALT = 0x0F, // HALT process
};

class Compiler {
private:
    // For storing strings
    std::vector<std::string> stringLiterals;
    uint8_t stringIdCounter = 0;
    // For storing variables
    std::unordered_map<std::string, uint8_t> symbolTable;
    uint8_t variableIdCounter = 0;
    // For tracking nested FOR loops
    uint8_t loopRegisterCounter = 0;

    void append(std::vector<uint8_t>& code, uint8_t op, std::initializer_list<uint8_t> operands);
    std::vector<std::string> tokenize(const std::string& line);

    uint8_t storeString(const std::string& literal);
    uint8_t storeVar(const std::string& name);

    // Helper for recursive compilation
    void compileInstructions(const std::vector<std::string>& lines, std::vector<uint8_t>& bytecode);

public:
    Compiler();
    std::vector<uint8_t> compile(const std::vector<std::string>& lines);
    std::vector<std::string> getStringLiterals();
};