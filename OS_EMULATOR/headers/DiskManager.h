#pragma once
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <iostream>
#include "Process.h"
#include "GlobalScheduler.h"

class DiskManager {
private:
    std::string backingFile = "csopesy-backing-store.txt";

    // Make singleton
    DiskManager();
    DiskManager(const DiskManager &) = delete;
    DiskManager &operator=(const DiskManager &) = delete;
    static DiskManager *sharedInstance;

    void removePage(int pid, int pageNum);
public:
    static DiskManager *getInstance();
    static void initialize();
    static void destroy();

    void storePage(int pid, int pageNum, const std::vector<uint8_t>& bytecode);
    std::vector<uint8_t> loadPage(int pid, int pageSize, int pageNumber);
    
    void clearBackingStore();
};