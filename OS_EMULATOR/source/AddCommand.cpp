#include "AddCommand.h"

// Private delegating constructor - all common logic here
AddCommand::AddCommand(int pid, const std::string& v1, 
                      std::variant<std::string, int> op2, 
                      std::variant<std::string, int> op3)
    : ICommand(pid, ADD), var1(v1), operand2(op2), operand3(op3) {
}

// Public constructors delegate to private one
AddCommand::AddCommand(int pid, const std::string& v1, const std::string& v2, const std::string& v3)
    : AddCommand(pid, v1, std::variant<std::string, int>(v2), std::variant<std::string, int>(v3)) {}

AddCommand::AddCommand(int pid, const std::string& v1, int val2, const std::string& v3)
    : AddCommand(pid, v1, std::variant<std::string, int>(val2), std::variant<std::string, int>(v3)) {}

AddCommand::AddCommand(int pid, const std::string& v1, const std::string& v2, int val3)
    : AddCommand(pid, v1, std::variant<std::string, int>(v2), std::variant<std::string, int>(val3)) {}

AddCommand::AddCommand(int pid, const std::string& v1, int val2, int val3)
    : AddCommand(pid, v1, std::variant<std::string, int>(val2), std::variant<std::string, int>(val3)) {}