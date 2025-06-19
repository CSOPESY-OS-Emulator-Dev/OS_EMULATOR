#pragma once
#include <iostream>
#include <thread>

class IETThread {
public:
    void start();
    static void sleep(int milliseconds);
protected:
    virtual void run() = 0;
private:
    std::thread thread;
};