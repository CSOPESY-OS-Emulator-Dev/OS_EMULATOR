#include "SleepCommand.h"

SleepCommand::SleepCommand(int pid, int duration) : ICommand(pid, SLEEP), duration(duration) {
    // Constructor initializes the SleepCommand with a process ID and duration.
}

void SleepCommand::execute(Process &process, std::string timeExecuted, int coreID) {
    if (duration > 0) {
        process.outputLog->push_back("[Sleeping] Duration Left: " + std::to_string(duration) + " | Progress Counter: " + std::to_string(process.progressCount));
        duration--;
        process.setState(SLEEPING);
    } else {
        process.progressCount++;
        isExecuted = true;
        process.setState(WAITING);
    }
}
