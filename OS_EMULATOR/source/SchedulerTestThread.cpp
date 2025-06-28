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
            IETThread::sleep(1); // Sleep for a short duration to avoid busy waiting
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
    // Generate instructions for the process
    auto instructions = generateInstructionsIterative(remaining, process->getProcessID(), processName); // Start with nested level 0
    // Add the generated instructions to the process
    process->instructionList = instructions;
    process->instructionCount = instructionCount; // ✅ Correct total
    
    return process; // Return the created process
}

std::vector<std::shared_ptr<ICommand>> SchedulerTestThread::generateInstructions(int& remainingExecs, int pid, const std::string& processName, int nestingLevel)
{
    std::vector<std::shared_ptr<ICommand>> instructionList;

    while (remainingExecs > 0) {
        bool canCreateFor = (nestingLevel < 3 && remainingExecs > 1);
        bool canCreateSleep = false;
        CommandType cmdType = getRandomCommandType(canCreateFor, canCreateSleep);

        if (cmdType == FOR && canCreateFor) {
            int maxIterations = std::min(remainingExecs - 1, 5); // reserve 1 for the ForCommand itself
            if (maxIterations <= 0) break;

            int iterations = getRandNum(1, maxIterations);

            int maxNestedExecs = (remainingExecs - 1) / iterations;
            if (maxNestedExecs <= 0) continue;

            int nestedRemainingExecs = getRandNum(1, maxNestedExecs);
            int actualNestedExecsUsed = nestedRemainingExecs;

            auto nestedInstructions = generateInstructions(actualNestedExecsUsed, pid, processName, nestingLevel + 1);

            int totalForCommandCost = 1 + iterations * actualNestedExecsUsed;
            if (totalForCommandCost > remainingExecs) break;

            instructionList.push_back(std::make_shared<ForCommand>(pid, nestedInstructions, iterations));
            remainingExecs -= totalForCommandCost;
        } else {
            auto simpleInstr = createInstruction(cmdType, pid, processName);
            if (simpleInstr) {
                instructionList.push_back(simpleInstr);
                remainingExecs--;
            }
        }
    }

    return instructionList;
}

std::vector<std::shared_ptr<ICommand>> SchedulerTestThread::generateInstructionsIterative(int& remainingExecs, int pid, const std::string& processName) {
    std::stack<InstructionFrame> frameStack;
    // Save initial count of remaining executions
    int initialRemainingExecs = remainingExecs;
    // Push the root frame
    frameStack.push({remainingExecs, 0, {}, 0, 0, false});

    std::vector<std::shared_ptr<ICommand>> finalInstructions;

    while (!frameStack.empty()) {
        auto& frame = frameStack.top();

        // If we have no more room, complete this frame
        if (frame.remainingExecs <= 0) {
            auto finishedInstructions = frame.instructions;
            frameStack.pop();

            if (!frameStack.empty() && frameStack.top().isForContext) {
                auto& parent = frameStack.top();
                int cost = 1 + frame.iterations * finishedInstructions.size();
                parent.remainingExecs -= cost;

                auto forCmd = std::make_shared<ForCommand>(pid, finishedInstructions, frame.iterations);
                parent.instructions.push_back(forCmd);
                parent.isForContext = false;
            } else {
                finalInstructions = std::move(finishedInstructions);
            }
            continue;
        }

        bool canNest = frame.nestingLevel < 3 && frame.remainingExecs > 5;
                                        // canNest, true
        auto cmdType = getRandomCommandType(canNest, true);

        if (cmdType == FOR && canNest) {
            int maxIter = std::min(5, frame.remainingExecs - 1);
            if (maxIter <= 0) continue;

            int iterations = getRandNum(1, maxIter);
            int maxSubExec = (frame.remainingExecs - 1) / iterations;
            if (maxSubExec <= 0) continue;

            int subExecs = getRandNum(1, maxSubExec);

            // Save FOR context
            frame.isForContext = true;

            // Push new frame for nested instructions
            frameStack.push({
                subExecs,
                frame.nestingLevel + 1,
                {},
                iterations,
                subExecs,
                false
            });
        } else {
            auto instr = createInstruction(cmdType, pid, processName);
            if (instr) {
                frame.instructions.push_back(instr);
                frame.remainingExecs--;
            }
        }
    }

    remainingExecs = initialRemainingExecs; // Update outer copy
    return finalInstructions;
}

std::shared_ptr<ICommand> SchedulerTestThread::createInstruction(CommandType commandType, int pid, std::string processName) {
    // Create a new instruction based on the command type
    std::string variable1 = "var" + std::to_string(getRandNum(0,65535));
    std::string variable2 = "var" + std::to_string(getRandNum(0,65535));
    std::string variable3 = "var" + std::to_string(getRandNum(0,65535));
    switch (commandType) {
        case PRINT:
            switch (getRandNum(0, 1)) {
                case 0:
                    return std::make_shared<PrintCommand>(pid, "Hello world from " + processName);
                case 1:
                    return std::make_shared<PrintCommand>(pid, "Value from " + variable1 + ": ", variable1);
                default:
                    return nullptr;
            }
        case DECLARE:
            return std::make_shared<DeclareCommand>(pid,variable1,getRandNum(0,65535) );
        case ADD:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<AddCommand>(pid,variable1,variable2,variable3);
            case 1:
                return std::make_shared<AddCommand>(pid,variable1,getRandNum(0,65535),variable3 );
            case 2:
                return std::make_shared<AddCommand>(pid,variable1,variable2,getRandNum(0,65535));
            case 3:
                return std::make_shared<AddCommand>(pid,variable1,getRandNum(0,65535),getRandNum(0,65535));
            default:
                return nullptr;
            }
            break;
        case SUBTRACT:
            switch (getRandNum(0,3))
            {
            case 0:
                return std::make_shared<SubtractCommand>(pid,variable1,variable2,variable3);
            case 1:
                return std::make_shared<SubtractCommand>(pid,variable1,getRandNum(0,65535),variable3);
            case 2:
                return std::make_shared<SubtractCommand>(pid,variable1,variable2,getRandNum(0,65535));
            case 3:
                return std::make_shared<SubtractCommand>(pid,variable1,getRandNum(0,65535),getRandNum(0,65535));
            default:
                return nullptr;
            }
        case SLEEP:
            // Placeholder for SLEEP command
            return std::make_shared<SleepCommand>(pid, getRandNum(0,255)); // Assuming 0 is the PID for the test
        default:
            return nullptr;
    }
}

CommandType SchedulerTestThread::getRandomCommandType(bool includeFOR, bool includeSLEEP) {
    int count = includeFOR ? TYPE_COUNT : TYPE_COUNT - 1; // Adjust count if FOR is excluded
    auto type = static_cast<CommandType>(getRandNum(0, count - 1)); // Generate a random command type between 1 and count - 1
    return includeSLEEP?type:type==SLEEP?static_cast<CommandType>(getRandNum(0, 3)):type;
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