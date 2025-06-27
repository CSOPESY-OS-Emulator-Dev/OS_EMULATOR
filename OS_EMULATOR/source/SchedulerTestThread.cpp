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
    while (process->getTotalIntstruction() < instructionCount) {
        // Here we would create a random instruction and add it to the process
        auto commandType = getRandomCommandType();
        auto instruction = createInstruction(commandType, process, instructionCount); // replace PRINT with commandType after implementing all command types
        process->addInstruction(instruction);
        process->incrementInstructionCount(); // Increment the instruction count for the process
    }
    
    return process; // Return the created process
}

std::shared_ptr<ICommand> SchedulerTestThread::createInstruction(CommandType commandType, std::shared_ptr<Process> process, int instructionCount, int nestedLevel = 0) {
    // Create a new instruction based on the command type
    switch (commandType) {
        case PRINT:
            return std::make_shared<PrintCommand>(std::to_string(process->getProcessID()), "Hello world from " + process->getProcessName()); // Assuming 0 is the PID for the test
        // Add cases for other command types as needed
        case DECLARE:
            return std::make_shared<DeclareCommand>(id,"var"+ std::to_string(getRandNum(0,10)),getRandNum(0,std::numeric_limits<uint16_t>::max()) );
        case ADD:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<AddCommand>(id,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0, 10)),"var" + std::to_string(getRandNum(0, 10)));
                break;
            case 1:
                return std::make_shared<AddCommand>(id,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),"var"+ std::to_string(getRandNum(0,10)) );
                break;
            case 2:
                return std::make_shared<AddCommand>(id,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10));
                break;
            case 3:
                return std::make_shared<AddCommand>(id,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),getRandNum(0,10));
                break;

            default:
                return nullptr;
                break;
            }
            // Placeholder for ADD command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case SUBTRACT:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<SubtractCommand>(id,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0, 10)),"var" + std::to_string(getRandNum(0, 10)));
                break;
            case 1:
                return std::make_shared<SubtractCommand>(id,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),"var"+ std::to_string(getRandNum(0,10)) );
                break;
            case 2:
                return std::make_shared<SubtractCommand>(id,"var" + std::to_string(getRandNum(0,10)),"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10));
                break;
            case 3:
                return std::make_shared<SubtractCommand>(id,"var" + std::to_string(getRandNum(0,10)),getRandNum(0,10),getRandNum(0,10));
                break;

            default:
                return nullptr;
                break;
            }
            // Placeholder for SUBTRACT command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case SLEEP:
            // Placeholder for SLEEP command
            return nullptr; // std::make_shared<ICommand>(id, commandType); // Assuming 0 is the PID for the test
        case FOR:
            // Placeholder for FOR command
            // Assuming we need to generate a ForCommand with a random number of instructions and iterations
            // Generate a random number of iterations based on the range between the instructionCount and getTotalInstructions within the process
            int range; //getRandomNumber(1, instructionCount - process->getTotalIntstruction());
            int iterations = rand() % (range); // Random iterations between 0 and range between instructionCount and total instructions
            int numInstructions = std::floor(static_cast<float>(range) / iterations); // Calculate the number of instructions per iteration
            return nullptr; // std::make_shared<ForCommand>(id, generateInstructions(process, numInstructions + process->getTotalInstruction(), nestedLevel++), iterations); // Assuming 0 is the PID for the test
        // Add more cases for other command types as needed
        default:
            return nullptr;
    }
}

std::vector<std::shared_ptr<ICommand>> SchedulerTestThread::generateInstructions(std::shared_ptr<Process> process, int instructionCount, int nestedLevel)
{
    std::vector<std::shared_ptr<ICommand>> instructions;
    // Generate a random instruction for each instruction count
    while(process->getTotalIntstruction() < instructionCount) {
        // Generate a vector of random instructions for the process
        auto commandType = getRandomCommandType(nestedLevel <= 3 ? true : false); // Limit FOR command nesting to 3 levels
        auto instruction = createInstruction(commandType, process, instructionCount, nestedLevel);
        instructions.push_back(instruction);
    }
    return instructions;
}

CommandType SchedulerTestThread::getRandomCommandType(bool includeFOR = true)
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
