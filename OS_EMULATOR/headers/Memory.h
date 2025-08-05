#pragma once
#include <iostream>
#include <memory>

class Memory {
public:
    Memory(size_t size);
    ~Memory();

    int read(int offset) const;
    int write(int offset, int value);
    size_t getMemorySize();

private:
    uint8_t* baseAddress;
    size_t size;
};