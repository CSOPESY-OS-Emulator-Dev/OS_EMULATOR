#pragma once
#include <iostream>
#include <memory>
#include "Process.h"

class IMemoryAllocator {
public:
    virtual void* allocate(Process &process) = 0;
    virtual void deallocate(Process &process) = 0;
    virtual std::string visualizeMemory() const = 0;
};