#include "DiskManager.h"
#include "GlobalScheduler.h"

DiskManager *DiskManager::sharedInstance = nullptr;

DiskManager::DiskManager() {}

void DiskManager::removePage(int pid, int pageNum)
{
    std::ifstream in(backingFile);
    std::ostringstream updated;
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int filePid, filePage;
        iss >> filePid >> filePage;

        if (filePid == pid && filePage == pageNum) {
            continue; // Skip the line (i.e., delete it)
        }
        updated << line << '\n';
    }

    in.close();
    // Replace backingLFile contents
    std::ofstream out(backingFile, std::ios::trunc);
    out << updated.str();
    out.close();
}

DiskManager *DiskManager::getInstance()
{
    return sharedInstance;
}

void DiskManager::initialize()
{
    // Initialize the DiskManager instance
    if (sharedInstance == nullptr)
    {
        sharedInstance = new DiskManager();
    }
}

void DiskManager::destroy()
{
    // Clean up the DiskManager instance
    if (sharedInstance != nullptr)
    {
        sharedInstance = nullptr;
    }
}

void DiskManager::storePage(int pid, int pageNum, const std::vector<uint8_t> &bytecode)
{
    std::ofstream out(backingFile, std::ios::app);

    // Store page in format:[PID] [PageNum] [PageSize Byte]
    out << pid << " " << pageNum;
    for (uint8_t byte : bytecode) {
        out << " " << static_cast<int>(byte);
    }
    out << "\n";
    out.close();

    // Get associated process and update page
    auto proc = GlobalScheduler::getInstance()->getProcessByID(pid);
    proc->pageTable[pageNum].inMemory = false;
}

std::vector<uint8_t> DiskManager::loadPage(int pid, int pageSize, int pageNumber)
{
    std::ifstream in(backingFile);
    std::string line;
    
    // Find page in backingstore
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int filePid, filePage;
        iss >> filePid >> filePage;

        if (filePid == pid && filePage == pageNumber) {
            std::vector<uint8_t> bytecode;
            int byte;

            // Get page bytecode
            while (iss >> byte) {
                // std::cout << "0x" << std::hex << byte << " ";
                bytecode.push_back(static_cast<uint8_t>(byte));
            }
            // std::cout << std::endl;

            // Pad with zeros if shorter than pageSize
            while (bytecode.size() < pageSize) bytecode.push_back(0);
            
            // Remove page from disk
            removePage(pid, pageNumber);

            return bytecode;
        }
    }

    // Return empty vector if not found
    in.close();
    return {};
}

void DiskManager::clearBackingStore()
{
    std::ofstream out(backingFile, std::ios::trunc); // Clear the file
    out.close();
}
