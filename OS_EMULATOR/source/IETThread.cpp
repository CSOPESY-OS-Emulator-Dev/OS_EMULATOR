#include "IETThread.h"

void IETThread::start() {
    thread = std::thread(&IETThread::run, this);
    thread.detach(); // Detach the thread to run independently
}

void IETThread::sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}