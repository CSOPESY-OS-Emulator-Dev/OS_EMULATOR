#include "SubtractCommand.h"

// Private delegating constructor - all common logic here
SubtractCommand::SubtractCommand(int pid, const std::string& v1, 
                      std::variant<std::string, int> op2, 
                      std::variant<std::string, int> op3)
    : ICommand(pid, ADD), var1(v1), operand2(op2), operand3(op3) {
}

void SubtractCommand::execute(Process& process, std::string timeExecuted, int coreID){

    int value2 = 0;
    int value3 = 0;
    if (std::holds_alternative<std::string>(this->operand2)) {
        std::string varName2 = std::get<std::string>(operand2);
        auto it = process.symbolTable.find(varName2);
        if(it != process.symbolTable.end()){
            value2 = it->second;
        }else{
            process.symbolTable[varName2]=0;
        }
    } else if (std::holds_alternative<int>(this->operand2)) {
        value2 = std::get<int>(operand2);
    }
    
    // Check what type op3 is
    if (std::holds_alternative<std::string>(this->operand3)) {
        std::string varName3 = std::get<std::string>(operand3);
        auto it = process.symbolTable.find(varName3);
        if(it != process.symbolTable.end()){
            value3 = it->second;
        }else{
            process.symbolTable[varName3]=0;
        }
    } else if (std::holds_alternative<int>(this->operand3)) {
        value3 = std::get<int>(operand3);
    }
    int result = value2 - value3;
    process.symbolTable[var1] = result;
}
// // Public constructors delegate to private one
// AddCommand::AddCommand(int pid, const std::string& v1, const std::string& v2, const std::string& v3)
//     : AddCommand(pid, v1, std::variant<std::string, int>(v2), std::variant<std::string, int>(v3)) {}

// AddCommand::AddCommand(int pid, const std::string& v1, int val2, const std::string& v3)
//     : AddCommand(pid, v1, std::variant<std::string, int>(val2), std::variant<std::string, int>(v3)) {}

// AddCommand::AddCommand(int pid, const std::string& v1, const std::string& v2, int val3)
//     : AddCommand(pid, v1, std::variant<std::string, int>(v2), std::variant<std::string, int>(val3)) {}

// AddCommand::AddCommand(int pid, const std::string& v1, int val2, int val3)
//     : AddCommand(pid, v1, std::variant<std::string, int>(val2), std::variant<std::string, int>(val3)) {}