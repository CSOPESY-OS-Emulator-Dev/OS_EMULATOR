#include "SchedulerTestThread.h"


SchedulerTestThread::SchedulerTestThread(int cpuCycle, int minIns, int maxIns, int minMemProc, int maxMemProc, int memPerFrame) 
        : cpuTick(0), cpuCycle(cpuCycle), minIns(minIns), maxIns(maxIns), minMemProc(minMemProc), maxMemProc(maxMemProc), memPerFrame(memPerFrame) {
    this->start();
    srand(time(0)); // Seed the random number generator
}

void SchedulerTestThread::run() {
    // This loop now correctly creates and assigns processes.
    while (true) {
        if (isRunning) {
            if (cpuTick % cpuCycle == 0) {
                // Create new Process Name
                // std::cout << "Creating process name" << std::endl;
                auto processName = "Process_" + std::to_string(processCount);
                // std::cout << "Name: " << processName << std::endl;
                // Generate a random number of memory in the power of 2
                // std::cout << "Generating random sized memory" << std::endl;
                int memorySize = getRandNum(maxMemProc, minMemProc);
                // Create new process
                // std::cout << "Creating new process" << std::endl;
                auto process = createProcess(processName, memorySize); // Using a fixed 4KB memory for example
                // Assign to scheduler
                // std::cout << "Assigning to scheduler" << std::endl;
                assignToScheduler(process);
            }
            IETThread::sleep(500);
            cpuTick++;
        }
    }
}

void SchedulerTestThread::stop() {
    isRunning = false;
}

void SchedulerTestThread::resume() {
    isRunning = true;
}

std::shared_ptr<Process> SchedulerTestThread::createProcess(std::string processName, int memorySize) {
    if (GlobalScheduler::getInstance()->processExists(processName)) {
        // If the process already exists return null
        return nullptr;
    }
    // Create new process
    auto process = std::make_shared<Process>(processName, processCount++, memorySize);
    // Generate a random number for the total count of instructions
    int instructionCount = getRandNum(minIns, maxIns);
    // Generate a random batch of instructions
    auto instructions = generateInstructions(instructionCount, processName, 0);
    // Create a new process
    process->instructionList = instructions;
    // Set the total instructions for tracking progress
    process->setTotalInstructions(instructionCount); 
    return process; 
}

std::shared_ptr<Process> SchedulerTestThread::createProcess(std::string processName, int memorySize, const std::vector<std::string> &lines) {
    if (GlobalScheduler::getInstance()->processExists(processName)) {
        // If the process already exists return null
        return nullptr;
    }
    auto process = std::make_shared<Process>(processName, processCount++, memorySize);
    process->instructionList = lines;
    return process; 
}

/**
 * @brief Generates a list of instructions with a specific total execution cost.
 * This is the corrected and simplified version of the function.
 * @param totalExecs The total number of effective instructions to generate.
 * @param processName The name of the process for PRINT statements.
 * @param nestingLevel The current recursion depth for FOR loops.
 * @return A vector of instruction strings.
 */
std::vector<std::string> SchedulerTestThread::generateInstructions(int totalExecs, const std::string& processName, int nestingLevel) {
    std::vector<std::string> instructionList;
    int remainingExecs = totalExecs;

    while (remainingExecs > 0) {
        bool canCreateFor = (nestingLevel < 3 && remainingExecs > 2); // A FOR loop must cost at least 2 (FOR [] 1)
        CommandType cmdType = getRandomCommandType(canCreateFor, false);

        if (cmdType == FOR) {
            // --- This is the new, corrected logic for FOR loops ---

            // 1. Decide on the loop's parameters.
            int availableForLoopContent = remainingExecs - 1; // Reserve 1 for the FOR instruction itself.
            int iterations = getRandNum(1, std::min(availableForLoopContent, 5)); // Cap iterations at 5 for sanity.
            
            // Distribute the available executions among the iterations.
            int maxExecsPerIteration = availableForLoopContent / iterations;
            if (maxExecsPerIteration == 0) continue; // Not enough remaining execs to do anything meaningful.
            
            int nestedExecsToGenerate = getRandNum(1, maxExecsPerIteration);

            // 2. Recursively generate the nested instructions.
            //    We pass the desired count BY VALUE.
            auto nestedInstructions = generateInstructions(nestedExecsToGenerate, processName, nestingLevel + 1);

            // 3. Calculate the true cost and build the instruction string.
            int totalCost = 1 + (iterations * nestedExecsToGenerate);
            
            std::stringstream forLoopString;
            forLoopString << "FOR [";
            for (size_t i = 0; i < nestedInstructions.size(); ++i) {
                forLoopString << nestedInstructions[i] << (i == nestedInstructions.size() - 1 ? "" : "; ");
            }
            forLoopString << "] " << iterations;
            
            instructionList.push_back(forLoopString.str());
            remainingExecs -= totalCost;

        } else {
            // Handle simple, single-cost instructions.
            auto simpleInstr = createInstruction(cmdType, processCount, processName);
            if (!simpleInstr.empty()) {
                instructionList.push_back(simpleInstr);
                remainingExecs--; // Simple instructions always cost 1.
            }
        }
    }
    return instructionList;
}

/**
 * @brief Creates a single instruction string with random parameters.
 * THIS VERSION RESTORES THE HARD REQUIREMENT of using the full 0-65535 range
 * for variable names and immediate values.
 */
std::string SchedulerTestThread::createInstruction(CommandType commandType, int pid, std::string processName) {
    // Generate variable names and values using the required large random range.
    std::string variable1 = "var" + std::to_string(getRandNum(0, 32));
    std::string variable2 = "var" + std::to_string(getRandNum(0, 32));
    std::string variable3 = "var" + std::to_string(getRandNum(0, 32));
    std::string immediate_val_1 = std::to_string(getRandNum(0, 65535));
    std::string immediate_val_2 = std::to_string(getRandNum(0, 65535));

    switch (commandType) {
        case PRINT:
            switch (getRandNum(0, 1)) {
                case 0:
                    return "PRINT(\"Hello from "  + processName + "\")";
                case 1:
                    return "PRINT(\"Value of " + variable1 + ": \", " + variable1 + ")";
            }
            break; // Should not be reached
        case DECLARE:
            return "DECLARE(" + variable1 + ", " + immediate_val_1 + ")";
        case ADD:
            switch (getRandNum(0, 3)) {
                case 0:
                    return "ADD(" + variable1 + ", " + variable2 + ", " + variable3 + ")";
                case 1:
                    return "ADD(" + variable1 + ", " + immediate_val_1 + ", " + variable3 + ")";
                case 2:
                    return "ADD(" + variable1 + ", " + variable2 + ", " + immediate_val_2 + ")";
                case 3:
                    return "ADD(" + variable1 + ", " + immediate_val_1 + ", " + immediate_val_2 + ")";
            }
            break; // Should not be reached
        case SUBTRACT:
            switch (getRandNum(0, 3)) {
                case 0:
                    return "SUBTRACT(" + variable1 + ", " + variable2 + ", " + variable3 + ")";
                case 1:
                    return "SUBTRACT(" + variable1 + ", " + immediate_val_1 + ", " + variable3 + ")";
                case 2:
                    return "SUBTRACT(" + variable1 + ", " + variable2 + ", " + immediate_val_2 + ")";
                case 3:
                    return "SUBTRACT(" + variable1 + ", " + immediate_val_1 + ", " + immediate_val_2 + ")";
            }
            break; // Should not be reached
        case SLEEP:
            return "SLEEP(" + std::to_string(getRandNum(0, 255)) + ")";
        default:
            return "";
    }
    return ""; // Return empty string for fall-through cases
}

// A clearer way to get a random command type.
CommandType SchedulerTestThread::getRandomCommandType(bool includeFOR, bool includeSLEEP) {
    CommandType type;
    do {
        int maxType = includeFOR ? TYPE_COUNT : TYPE_COUNT - 1;
        type = static_cast<CommandType>(getRandNum(0, maxType - 1));
    } while (!includeSLEEP && type == SLEEP);
    return type;
}

int SchedulerTestThread::getRandNum(int min, int max){
   if (min > max) std::swap(min, max);
   return rand() % (max - min + 1) + min;
}

void SchedulerTestThread::assignToScheduler(std::shared_ptr<Process> process) {
    // Add the process to the scheduler's queue
    GlobalScheduler::getInstance()->queueProcess(process);
    // Add the process to the process map
    GlobalScheduler::getInstance()->addProcess(process);
}