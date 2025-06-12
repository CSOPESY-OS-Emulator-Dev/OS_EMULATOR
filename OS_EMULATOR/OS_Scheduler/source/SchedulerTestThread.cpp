#include "SchedulerTestThread.h"

SchedulerTestThread::SchedulerTestThread(int cpuCycle, int minIns, int maxIns) 
        : cpuTick(0), cpuCycle(cpuCycle), minIns(minIns), maxIns(maxIns) {
    // Constructor initializes the CPU cycle and instruction limits for the test thread
}

void SchedulerTestThread::run() {
    while (isRunning) {
        if(cpuTick % cpuCycle == 0) {
            // Create new processes only every cpuCycle
            // Create a new process every CPU cycle
            auto process = createProcess("Process_" + std::to_string(processCount));
            // Assign new process to scheduler
            assignToScheduler(process);
            // Log process creation
            std::cout << "Created Process: " << process->getProcessName() 
                      << " with ID: " << process->getProcessID() 
                      << " at CPU Tick: " << cpuTick << std::endl;
            // Sleep for the specified CPU cycle duration
            IETThread::sleep(10); // Sleep for a short duration to avoid busy waiting
        }
        cpuTick++;
    }
}

void SchedulerTestThread::stop() {
    isRunning = false; // Set the running flag to false to stop the thread
}

void SchedulerTestThread::resume() {
    isRunning = true; // Set the running flag to true to resume the thread
    this->start(); // Start the thread if it was stopped
}

std::shared_ptr<Process> SchedulerTestThread::createProcess(std::string processName) {
    // This condition allows the main thread to retrieve an existing process if it already exists
    // The command screen -s <processName> will retrieve the process by its name
    // If the process does not exist, it will create a new one with the given name and new ID
    // Check if processName exists in the process map
    if (GlobalScheduler::getInstance()->processExists(processName)) {
        // If the process already exists, retrieve it
        auto existingProcess = GlobalScheduler::getInstance()->getProcessByName(processName);
        return existingProcess;
    }
    // Create a new process with the given name and ID
    auto process = std::make_shared<Process>(processName, processCount++);
    
    // Randomly generate instructions for the process
    int instructionCount = rand() % (maxIns - minIns + 1) + minIns;
    for (int i = 0; i < instructionCount; ++i) {
        // Here we would create a random instruction and add it to the process
        // auto commandType = getRandomCommandType() <-- uncomment this part after implementing all command types
        auto instruction = createInstruction(PRINT, processName, processCount); // replace PRINT with commandType after implementing all command types
        process->addInstruction(instruction);
    }
    
    return process; // Return the created process
}

std::shared_ptr<ICommand> SchedulerTestThread::createInstruction(CommandType commandType, std::string processName, int id) {
    // Create a random instruction of the specified command type
    switch (commandType) {
        case PRINT:
            return std::make_shared<PrintCommand>(id, "Hello world from " + processName); // Assuming 0 is the PID for the test
        // Add cases for other command types as needed
        case DECLARE:
            // Placeholder for DECLARE command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case ADD:
            // Placeholder for ADD command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case SUBTRACT:
            // Placeholder for SUBTRACT command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case SLEEP:
            // Placeholder for SLEEP command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case FOR:
            // Placeholder for FOR command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        // Add more cases for other command types as needed
        default:
            return nullptr;
    }
}

CommandType SchedulerTestThread::getRandomCommandType() {
    static std::random_device rd;   // Random seed
    static std::mt19937 gen(rd());  // Mersenne Twister RNG
    static std::uniform_int_distribution<> dist(0, TYPE_COUNT - 1);

    return static_cast<CommandType>(dist(gen));
}

void SchedulerTestThread::assignToScheduler(std::shared_ptr<Process> process) {
    // Add the process to the scheduler's queue
    GlobalScheduler::getInstance()->queueProcess(process);
    // Add the process to the process map
    GlobalScheduler::getInstance()->addProcess(process);
}
