#include "DeclareCommand.h"

DeclareCommand::DeclareCommand(int pid, const std::string& variable, uint16_t value) 
    : ICommand(pid,DECLARE), variable(variable), value(value){
}

void DeclareCommand::execute(Process& process, std::string timeExecuted, int coreID){
    auto it = process.symbolTable.find(this->variable);

    if(it != process.symbolTable.end()){
    it->second = value;
    }else{
        process.symbolTable[variable]=value;
    }
    // Log the declaration
    process.outputLog->push_back(timeExecuted + " Core:" + std::to_string(coreID) + " Declared variable \"" + variable + "\" with value " + std::to_string(value) + ".");
    process.progressCount++;
    isExecuted = true;
}

