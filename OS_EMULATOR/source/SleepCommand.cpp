#include "SleepCommand.h"

SleepCommand::SleepCommand(int pid, int duration) : ICommand(pid, SLEEP), duration(duration) {
    // Constructor initializes the SleepCommand with a process ID and duration.
}

void SleepCommand::execute(Process &process, std::string timeExecuted, int coreID) {
    if (duration > 0) {
        duration--;
    } else {
        process.progressCount++;
        isExecuted = true;
    }
}
