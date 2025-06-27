#include "SchedulerTestThread.h"

SchedulerTestThread::SchedulerTestThread(int cpuCycle, int minIns, int maxIns) 
        : cpuTick(0), cpuCycle(cpuCycle), minIns(minIns), maxIns(maxIns) {
    // Constructor initializes the CPU cycle and instruction limits for the test thread
    this->start();
}

void SchedulerTestThread::run() {
    while (true) {
        if (isRunning == true) {
            if(cpuTick % cpuCycle == 0) {
                // Create new processes only every cpuCycle
                // Create a new process every CPU cycle
                auto process = createProcess("Process_" + std::to_string(processCount));
                // Assign new process to scheduler
                assignToScheduler(process);
            }
            // Sleep for the specified CPU cycle duration
            IETThread::sleep(10); // Sleep for a short duration to avoid busy waiting
            cpuTick++;
        }
    }
}

void SchedulerTestThread::stop() {
    isRunning = false; // Set the running flag to false to stop the thread
}

void SchedulerTestThread::resume() {
    isRunning = true; // Set the running flag to true to resume the thread
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
    int remaining = instructionCount;
    process->incrementInstructionCount(instructionCount);
    auto instructions = generateInstructions(remaining, process->getProcessID(), processName, 0); // Start with nested level 0
    
    // Add the generated instructions to the process
    for (auto& instr : instructions) {
        process->addInstruction(instr);
    }
    return process; // Return the created process
}

std::shared_ptr<ICommand> SchedulerTestThread::createInstruction(CommandType commandType, int pid, std::string processName) {
    // Create a new instruction based on the command type
    switch (commandType) {
        case PRINT:
            switch (getRandNum(0, 1)) {
                case 0:
                    return std::make_shared<PrintCommand>(pid, "Hello world from " + processName);
                case 1:
                    return std::make_shared<PrintCommand>(pid, "Value from " + processName, "var" + std::to_string(getRandNum(0, 10)));
                default:
                    return nullptr;
            }
        case DECLARE:
            return std::make_shared<DeclareCommand>(pid,"var"+ std::to_string(getRandNum(0,10)),getRandNum(0,std::numeric_limits<uint16_t>::max()) );
        case ADD:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<AddCommand>(pid,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0, 10)),"var" + std::to_string(getRandNum(0, 10)));
            case 1:
                return std::make_shared<AddCommand>(pid,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),"var"+ std::to_string(getRandNum(0,10)) );
            case 2:
                return std::make_shared<AddCommand>(pid,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10));
            case 3:
                return std::make_shared<AddCommand>(pid,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),getRandNum(0,10));
            default:
                return nullptr;
            }
            break;
        case SUBTRACT:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<SubtractCommand>(pid,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0, 10)),"var" + std::to_string(getRandNum(0, 10)));
            case 1:
                return std::make_shared<SubtractCommand>(pid,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),"var"+ std::to_string(getRandNum(0,10)) );
            case 2:
                return std::make_shared<SubtractCommand>(pid,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10));
            case 3:
                return std::make_shared<SubtractCommand>(pid,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),getRandNum(0,10));
            default:
                return nullptr;
            }
        case SLEEP:
            // Placeholder for SLEEP command
            return std::make_shared<PrintCommand>(pid, "Hello world from " + processName); // Assuming 0 is the PID for the test
        default:
            return nullptr;
    }
}

std::vector<std::shared_ptr<ICommand>> SchedulerTestThread::generateInstructions(int& remaining, int pid, std::string processName, int nestedLevel)
{
    std::vector<std::shared_ptr<ICommand>> instructions;
    // Generate instructions until the remaining count is zero
    while (remaining > 0) {
        // Choose a random command type
        auto commandType = getRandomCommandType(false); // Include FOR command only after 3 nesting levels
        auto instruction = createInstruction(commandType, pid, processName);
        // std::cout << "Generating instruction: " << commandType << " for process: " << processName << std::endl;
        // If the instruction is valid, add it to the list and decrease the remaining count
        if (instruction) {
            instructions.push_back(instruction);
            remaining--;
        }
        // If the command type is FOR, generate nested instructions
        // if (commandType == FOR && nestedLevel <= 3) {
        //     int range = getRandNum(1, remaining);
        //     int iterations = getRandNum(0, range - 1); // Random number of iterations between 0 to range -1
        //     int maxInstructions = getRandNum(0, iterations == 0 ? 0 : (range - 1) / iterations); // Randomly determine the number of instructions for the FOR loop
        //     // Display rang, iterations, randNum and maxInstructions for debugging
        //     std::cout << std::flush;
        //     std::cout << "Generating FOR command with range: " << range 
        //               << ", iterations: " << iterations
        //               << ", maxInstructions: " << maxInstructions 
        //               << ", nested level: " << nestedLevel << std::endl;
        //     auto nestedInstructions = generateInstructions(maxInstructions, pid, processName, nestedLevel + 1); // Generate nested instructions
        //     // std::cout << "Instruction Count: " << nestedInstructions.size() << std::endl;
            
        //     // Add the FOR command with nested instructions
        //     auto forCommand = std::make_shared<ForCommand>(pid, nestedInstructions, iterations);
        //     instructions.push_back(forCommand);
        //     remaining -= (maxInstructions * iterations) + 1; // Decrease the remaining count by the number of nested instructions
        // }
    }
    return instructions;
}

CommandType SchedulerTestThread::getRandomCommandType(bool includeFOR)
{
    static std::random_device rd;   // Random seed
    static std::mt19937 gen(rd());  // Mersenne Twister RNG
    static const int count = includeFOR ? TYPE_COUNT : TYPE_COUNT - 1; // Adjust count if FOR is excluded
    static std::uniform_int_distribution<> dist(0, count - 1);

    return static_cast<CommandType>(dist(gen));
}

int SchedulerTestThread::getRandNum(int min, int max){
   return rand() % (max - min + 1) + min;
}
void SchedulerTestThread::assignToScheduler(std::shared_ptr<Process> process) {
    // Add the process to the scheduler's queue
    GlobalScheduler::getInstance()->queueProcess(process);
    // Add the process to the process map
    GlobalScheduler::getInstance()->addProcess(process);
    
}


