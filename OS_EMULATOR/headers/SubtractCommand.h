#pragma once
#include <string>
#include <variant>
#include "ICommand.h"
#include "Process.h"
#include "GlobalScheduler.h"
class SubtractCommand : public ICommand {
private:
    std::string var1;
    std::variant<std::string, int> operand2;
    std::variant<std::string, int> operand3;
    
    // Private delegating constructor
    // AddCommand(int pid, const std::string& v1, 
    //            std::variant<std::string, int> op2, 
    //            std::variant<std::string, int> op3);

public:
    SubtractCommand(int pid, const std::string& v1, 
               std::variant<std::string, int> op2, 
               std::variant<std::string, int> op3);
    void execute(Process& process, std::string timeExecuted, int coreID) override;
    // AddCommand(int pid, const std::string& v1, const std::string& v2, const std::string& v3);
    // AddCommand(int pid, const std::string& v1, int val2, const std::string& v3);
    // AddCommand(int pid, const std::string& v1, const std::string& v2, int val3);
    // AddCommand(int pid, const std::string& v1, int val2, int val3);
};