#include "Memory.h"

Memory::Memory(size_t size) : size(size) {
    baseAddress = new uint8_t[size * sizeof(uint8_t)];
    for (size_t i = 0; i < size; ++i) {
        baseAddress[i] = 0x00; // Initialize memory to zero
    }
}

Memory::~Memory() {
    delete[] baseAddress;
}

int Memory::read(int offset) const {
    if (offset < 0 || offset >= size) {
        return -1; // Access Violation Error
    }
    return baseAddress[offset];
}

int Memory::write(int offset, int value) {
    if (offset < 0 || offset >= size) {
        return -1; // Access Violation Error
    }
    baseAddress[offset] = static_cast<uint8_t>(value);
    return 0; // Success
}