#include "CoreThread.h"

CoreThread::CoreThread(int id, int cpuCycle) : coreID(id), cpuCycle(cpuCycle), cpuTicks(0), activeTicks(0), currentTicks(0) {
    this->currentProcess = nullptr; // Initialize current process to nullptr
    this->occupied = false; // Initially, the core is not occupied
    this->mm = MemoryManager::getInstance();
}

void CoreThread::run() {
    // This method runs in a loop, simulating the core's operation.
    // A delay is introduced using a cpu cycle to control the execution speed.
    while (this->isRunning) {
        // If the core is occupied and the current process is set, check if it can execute
        if (this->occupied && this->currentProcess && this->cpuTicks % (this->cpuCycle + 1) == 0) {
            // Check if the process has finished executing
            if (this->currentProcess->getState() == ACCESSVIOLATION) {
                // Push current process to error processes in GlobalScheduler
                GlobalScheduler::getInstance()->finishProcess(this->currentProcess);
                releaseProcess();
            } else if (this->currentProcess->getState() == WAITING) {
                // Queue current process back to scheduler in GlobalScheduler
                GlobalScheduler::getInstance()->queueProcess(this->currentProcess);
                releaseProcess();
            } else if (this->currentProcess->getState() == FINISHED) {
                // Push current process to finished processes in GlobalScheduler
                GlobalScheduler::getInstance()->finishProcess(this->currentProcess);
                releaseProcess();
            } else if (this->currentTicks <= 0) {
                // Queue current process back to scheduler in GlobalScheduler
                GlobalScheduler::getInstance()->queueProcess(this->currentProcess);
                releaseProcess();
            } else if (this->currentProcess->getState() == SLEEPING && this->currentProcess->sleepDuration > 0) {
                // If process is currently executing a sleep instruction
                this->currentProcess->sleepDuration--;
                this->currentTicks--; // Decrease the ticks for the current process
                if (this->currentProcess->sleepDuration == 0) this->currentProcess->setState(WAITING);
            } else {
                // If the process is still running and has ticks left, execute its instruction
                uint16_t PC = currentProcess->programCounter;
                executeByteCode(PC);
                if (this->currentProcess->getState() == FINISHED) continue;
                currentProcess->progressCounter++;
                this->currentTicks--; // Decrease the ticks for the current process
            } 
            this->activeTicks++;
        }
        this->cpuTicks++;
        sleep(1); // Sleep for 10 millisecond to simulate time passing
    }
}

void CoreThread::stop() {
    this->isRunning = false; // Stop the core thread
    // std::cout << "Core " << this->coreID << " stopped." << std::endl; // Log the stop action
}

void CoreThread::assignProcess(std::shared_ptr<Process> process, int ticks) {
    this->currentProcess = process;
    this->currentProcess->setCoreID(this->coreID); // Set the core ID for the process
    this->currentProcess->setState(RUNNING); // Set the process state to RUNNING
    
    this->activeTicks += ticks;
    this->currentTicks = ticks;
    this->occupied = true;
    
    // std::cout << "Process " << this->currentProcess->getProcessID() << " assigned to Core " << this->coreID << " with " << ticks << " ticks." << std::endl;
}

bool CoreThread::isOccupied() const {
    return this->occupied; // Return whether the core is currently occupied
}

std::string CoreThread::getProcess() {
    // Return a string representation of the current process running on this core
    return currentProcess->getProcessName() + "    " + 
           currentProcess->getTimeRunning() + "    Core: " + 
           std::to_string(coreID) + "    " + 
           std::to_string(currentProcess->getProgressCount()) + " / " + 
           std::to_string(currentProcess->getTotalIntstruction());
}

void CoreThread::releaseProcess()
{
    this->occupied = false; // Free the core
    this->currentProcess = nullptr; // Clear the current process
    this->currentTicks = 0; // Reset current ticks
}

// Replace the entire executeByteCode function in CoreThread.cpp with this corrected version.
void CoreThread::executeByteCode(uint16_t PC) {
    // Helper lambda to fetch a byte and advance the process's PC
    auto fetch = [&]() -> uint8_t {
        uint8_t byte = mm->fetchByte(currentProcess, currentProcess->programCounter);
        currentProcess->programCounter++;
        return byte;
    };
    
    // Helper lambda to fetch a 16-bit word
    auto fetch16 = [&]() -> uint16_t {
        uint8_t low = fetch();
        uint8_t high = fetch();
        return (static_cast<uint16_t>(high) << 8) | low;
    };
    
    int byteInstr = static_cast<int>(mm->fetchByte(currentProcess, PC));
    // std::cout << "Program Counter: 0x" << std::hex << PC << " Opcode: 0x" << byteInstr << std::endl;
    currentProcess->programCounter++; // Consume the opcode
    int variant = byteInstr & 0xF0;
    OpCode OP = static_cast<OpCode>(byteInstr & 0x0F);

    switch (OP) {
        case OpCode::HALT:
            // std::cout << "HALT Process" << std::endl;
            currentProcess->setState(FINISHED);
            break;

        case OpCode::NOP:
            // std::cout << "NOP Process" << std::endl;
            // PC is already advanced, do nothing else.
            break;

        case OpCode::PRINT: {
            // std::cout << "PRINT Process" << std::endl;
            uint8_t strID = fetch();
            if (variant == 0x00) { // PRINT(msg.id)
                currentProcess->outputLog->push_back(getFormattedCurrentTime() + " Core:" + std::to_string(coreID) + " \"" + currentProcess->stringLiterals[strID] + "\"");
            } else { // PRINT(msg.id, var)
                uint8_t varID = fetch();
                uint16_t value = (varID == 0xFF) ? 0 : mm->read16(currentProcess, static_cast<uint16_t>(varID * 2));
                currentProcess->outputLog->push_back(getFormattedCurrentTime() + " Core:" + std::to_string(coreID) + " \"" + currentProcess->stringLiterals[strID] + std::to_string(value) + "\"");
            }
            break;
        }

        case OpCode::DECLARE: {
            // std::cout << "DECLARE Process" << std::endl;
            uint8_t varID = fetch();
            uint16_t value = fetch16();
            mm->write16(currentProcess, static_cast<uint16_t>(varID * 2), value);
            break;
        }

        case OpCode::ADD:
        case OpCode::SUBTRACT: {
            // std::cout << "ADD|SUBTRACT Process. Variant: " << std::dec << variant << std::endl;
            uint8_t destID = fetch();
            uint16_t val1 = (variant == 0x20 | variant == 0x30) ? fetch16() : mm->read16(currentProcess, fetch() * 2);
            uint16_t val2 = (variant == 0x10 | variant == 0x30) ? fetch16() : mm->read16(currentProcess, fetch() * 2);
            uint16_t result = (OP == OpCode::ADD) ? (val1 + val2) : (val1 - val2);
            mm->write16(currentProcess, static_cast<uint16_t>(destID * 2), result);
            break;
        }

        case OpCode::WRITE: {
            // std::cout << "WRITE Process" << std::endl;
            uint16_t address = fetch16();
            uint16_t value = (variant != 0x10) ? fetch16() : mm->read16(currentProcess, fetch() * 2);
            // Validate Address
            if(address < currentProcess->getByteSize() || address > currentProcess->getMemorySize()) {
                std::stringstream hex;
                hex << "0x"                          // Add the "0x" prefix
                    << std::hex                       // Set the stream to output in hexadecimal format
                    << std::uppercase                // Use uppercase letters for hex (A-F)
                    << std::setfill('0')              // Set the padding character to '0'
                    << std::setw(4)                   // Set the total width of the number to 4 digits
                    << address; 
                currentProcess->setState(ACCESSVIOLATION); // Siginify Memory Access Violation
                currentProcess->invalidAddress = hex.str();
                return;
            }
            mm->write16(currentProcess, address, value);
            break;
        }

        case OpCode::READ: {
            // std::cout << "READ Process" << std::endl;
            uint8_t varID = fetch();
            uint16_t address = fetch16();
            uint16_t value = mm->read16(currentProcess, address);
            if(address < currentProcess->getByteSize() || address > currentProcess->getMemorySize()) {
                std::stringstream hex;
                hex << "0x"                          // Add the "0x" prefix
                    << std::hex                       // Set the stream to output in hexadecimal format
                    << std::uppercase                // Use uppercase letters for hex (A-F)
                    << std::setfill('0')              // Set the padding character to '0'
                    << std::setw(4)                   // Set the total width of the number to 4 digits
                    << address; 
                currentProcess->setState(ACCESSVIOLATION); // Siginify Memory Access Violation
                currentProcess->invalidAddress = hex.str();
                return;
            }
            mm->write16(currentProcess, varID * 2, value);
            break;
        }

        case OpCode::SLEEP: {
            // std::cout << "SLEEP Process" << std::endl;
            currentProcess->sleepDuration = fetch();
            currentProcess->setState(SLEEPING);
            break;
        }

        case OpCode::MOV: {
            // std::cout << "MOV Process" << std::endl;
            uint8_t regID = fetch();
            uint8_t value = fetch();
            currentProcess->registers.getRegister(regID) = value;
            break;
        }

        case OpCode::JMP: { // New, simple handler for unconditional JMP
            uint16_t jumpAddress = fetch16();
            currentProcess->programCounter = jumpAddress;
            currentProcess->progressCounter--;
            break;
        }

        case OpCode::JMP_ZERO: { // Cleaned-up handler for conditional JMP_ZERO
            uint8_t regID = fetch();
            uint16_t jumpAddress = fetch16();
            uint16_t& regValue = currentProcess->registers.getRegister(regID); // This is now safe

            if (regValue == 0) {
                currentProcess->programCounter = jumpAddress;
            } else {
                regValue--;
            }
            currentProcess->progressCounter--;
            break;
        }

        default:
            currentProcess->progressCounter--;
            // std::cerr << "Error: Unknown OpCode " << std::hex << (int)byteInstr << std::endl;
            currentProcess->setState(FINISHED); // Halt on error
            break;
    }
}

std::string CoreThread::getFormattedCurrentTime()
{
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();
    auto in_time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to local time
    std::tm* localTime = std::localtime(&in_time_t); // Use localtime_s on Windows

    // Format the output
    std::ostringstream oss;
    oss << "(" << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S")
        << '.' << std::setw(3) << std::setfill('0') << ms.count()
        << ' ' << std::put_time(localTime, "%p") << ")";
    return oss.str();
}

std::string CoreThread::getUsedProcessMemory(){
    std::stringstream ss;
    int count = 0;
    auto pageTable = currentProcess->pageTable; 
    // std::cout<<this->currentProcess.pageTable.size()<<std::endl;
    for(int i=0; pageTable.size();i++){
        if(pageTable[i].inMemory){
            count++;
        }
    }
    ss << currentProcess->getProcessName() << " " << std::to_string(count*mm->getPageSize());
    return ss.str();
}
