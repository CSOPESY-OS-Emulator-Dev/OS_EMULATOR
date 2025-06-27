#include "SleepCommand.h"

SleepCommand::SleepCommand(int pid, int duration) : ICommand(pid, SLEEP), duration(duration) {
    // Constructor initializes the SleepCommand with a process ID and duration.
}

void SleepCommand::execute(Process &process, std::string timeExecuted, int coreID) {
    process.setState(SLEEPING); // Set the process state to SLEEPING
    process.sleepDuration = duration; // Set the sleep duration for the process
}
