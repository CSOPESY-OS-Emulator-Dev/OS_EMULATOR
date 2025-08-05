#include "Compiler.h"

void Compiler::append(std::vector<uint8_t> &code, uint8_t op, std::initializer_list<uint8_t> operands)
{
    code.push_back(op);
    code.insert(code.end(), operands.begin(), operands.end());
}

/**
 * @brief Tokenizes a line of code using a single, unified state machine.
 * This function is robust and flexible, correctly parsing all specified formats:
 *  - INSTRUCTION(Param1, Param2) - with or without a space after INSTRUCTION.
 *  - INSTRUCTION Param1 Param2
 *  - INSTRUCTION Param1 + Param2
 * It treats spaces, commas, plus signs, and parentheses as delimiters, while
 * respecting protected blocks for string literals ("...") and FOR loop lists ([...]).
 */
std::vector<std::string> Compiler::tokenize(const std::string &line)
{
    std::vector<std::string> tokens;
    if (line.empty()) {
        return tokens;
    }

    std::string current_token;
    bool in_string = false;
    int bracket_level = 0;

    // We handle the trailing semicolon by trimming it off the input line first.
    std::string clean_line = line;
    if (!clean_line.empty() && clean_line.back() == ';') {
        clean_line.pop_back();
    }

    // A single, unified loop is more robust than multiple parsing stages.
    for (char c : clean_line) {
        // First, manage state for protected blocks.
        if (c == '"') {
            in_string = !in_string;
        } else if (!in_string) { // Only check brackets if not inside a string
            if (c == '[') bracket_level++;
            else if (c == ']') bracket_level--;
        }

        // Check for delimiters ONLY if we are not in a protected block.
        if (!in_string && bracket_level == 0) {
            // Case 1: The character is a standard delimiter (space, comma, plus).
            if (std::isspace(c) || c == ',' || c == '+') {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
                continue; // Skip the delimiter character itself.
            }
            // Case 2: The character is a parenthesis. This ALSO acts as a delimiter.
            if (c == '(' || c == ')') {
                // If there's a token before the parenthesis (like "DECLARE"), push it.
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
                continue; // Skip the parenthesis character itself.
            }
        }

        // If the character is not a delimiter, append it to the current token.
        current_token += c;
    }

    // After the loop, add the last token if it exists.
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;
}

uint8_t Compiler::storeString(const std::string &literal)
{
    auto it = std::find(stringLiterals.begin(), stringLiterals.end(), literal);
    if (it != stringLiterals.end()) {
        return std::distance(stringLiterals.begin(), it); // Return ID of existing literal
    } else {
        stringLiterals.push_back(literal); // add new string literal
        return stringIdCounter++; // return ID
    }
}

uint8_t Compiler::storeVar(const std::string &name)
{
    if (symbolTable.count(name)) {
        return symbolTable[name];
    } 
    if (variableIdCounter >= 32) { // Max Variables
        return 0xFF; // return INVALID ID
    }
    symbolTable[name] = variableIdCounter;
    return variableIdCounter++; // return new ID
}

Compiler::Compiler() {}

std::vector<uint8_t> Compiler::compile(const std::vector<std::string> &lines)
{
    // Initialize vector bytecode and reserve 64 bytes for variable values (Symbol Table)
    std::vector<uint8_t> bytecode;
    bytecode.reserve(64);
    for (int i = 0; i < 64; ++i) bytecode.push_back(0);

    // Initialize utilities
    stringLiterals.clear();
    symbolTable.clear();
    stringIdCounter = 0;
    variableIdCounter = 0;
    loopRegisterCounter = 0;

    // Start compilation process
    compileInstructions(lines, bytecode);
    // End of instruction
    bytecode.push_back(static_cast<uint8_t>(OpCode::HALT));
    return bytecode;
}

std::vector<std::string> Compiler::getStringLiterals()
{
    return stringLiterals;
}

void Compiler::compileInstructions(const std::vector<std::string>& lines, std::vector<uint8_t>& bytecode)
{
    for (const auto& line : lines) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;

        std::string instr = tokens[0];
        int tokenSize = tokens.size();

        if (instr == "PRINT") {
            switch (tokenSize) {
                case 2: // PRINT("msg")
                    append(bytecode, static_cast<uint8_t>(OpCode::PRINT), {storeString(tokens[1].substr(1, tokens[1].size() - 2))});
                    break;
                case 3: // PRINT("msg", var)
                    append(bytecode, static_cast<uint8_t>(OpCode::PRINT) | 0x10, {storeString(tokens[1].substr(1, tokens[1].size() - 2)), storeVar(tokens[2])});
                    break;
            }
        } else if (instr == "DECLARE") {
            uint8_t varID = storeVar(tokens[1]);
            uint16_t val = std::stoi(tokens[2]);
            if (varID != 0xFF) {
                append(bytecode, static_cast<uint8_t>(OpCode::DECLARE), {
                    varID, 
                    static_cast<uint8_t>(val & 0xFF), // lowbyte
                    static_cast<uint8_t>(val >> 8)}); // highbyte
            } else {
                append(bytecode, static_cast<uint8_t>(OpCode::NOP), {});
            }
        } else if (instr == "ADD" || instr =="SUBTRACT") {
            uint8_t var1 = storeVar(tokens[1]);
            if (var1 != 0xFF) {
                auto var2 = std::all_of(tokens[2].begin(), tokens[2].end(), ::isdigit) 
                    ? std::variant<std::string, int>(std::stoi(tokens[2]))
                    : std::variant<std::string, int>(tokens[2]);
                auto var3 = std::all_of(tokens[3].begin(), tokens[3].end(), ::isdigit) 
                    ? std::get<int>(std::variant<std::string, int>(std::stoi(tokens[3])))
                    : std::variant<std::string, int>(tokens[3]);
                auto variant = std::holds_alternative<int>(var2) 
                    ? std::holds_alternative<int>(var3) 
                        ? 0x30 
                        : 0x20
                    : std::holds_alternative<int>(var3)
                        ? 0x10 
                        : 0x00;

                std::holds_alternative<int>(var2) 
                    ? std::holds_alternative<int>(var3) 
                        ? append(bytecode, static_cast<uint8_t>(instr == "ADD" ? OpCode::ADD : OpCode::SUBTRACT) | variant, { 
                            var1, 
                            static_cast<uint8_t>(std::get<int>(var2) & 0xFF), 
                            static_cast<uint8_t>(std::get<int>(var2) >> 8), 
                            static_cast<uint8_t>(std::get<int>(var3) & 0xFF), 
                            static_cast<uint8_t>(std::get<int>(var3) >> 8)
                        }) 
                        : append(bytecode, static_cast<uint8_t>(instr == "ADD" ? OpCode::ADD : OpCode::SUBTRACT) | variant, { 
                            var1, 
                            static_cast<uint8_t>(std::get<int>(var2) & 0xFF), 
                            static_cast<uint8_t>(std::get<int>(var2) >> 8), 
                            storeVar(std::get<std::string>(var3))
                        }) 
                    : std::holds_alternative<int>(var3)
                        ? append(bytecode, static_cast<uint8_t>(instr == "ADD" ? OpCode::ADD : OpCode::SUBTRACT) | variant, { 
                            var1, 
                            storeVar(std::get<std::string>(var2)), 
                            static_cast<uint8_t>(std::get<int>(var3) & 0xFF), 
                            static_cast<uint8_t>(std::get<int>(var3) >> 8)
                        }) 
                        : append(bytecode, static_cast<uint8_t>(instr == "ADD" ? OpCode::ADD : OpCode::SUBTRACT) | variant, { 
                            var1, 
                            storeVar(std::get<std::string>(var2)), 
                            storeVar(std::get<std::string>(var3))
                        });
            } else {
                append(bytecode, static_cast<uint8_t>(OpCode::NOP), {}); // Skip Instruction
            }
        } else if (instr == "WRITE") {
            uint16_t address = std::stoi(tokens[1]);
            uint16_t value = std::stoi(tokens[2]);
            append(bytecode, static_cast<uint8_t>(OpCode::WRITE), {
                static_cast<uint8_t>(address & 0xFF), // lowbyte
                static_cast<uint8_t>(address >> 8), // highbyte
                static_cast<uint8_t>(value & 0xFF), // lowbyte
                static_cast<uint8_t>(value >> 8) // highbyte
            }); // highbyte
        } else if (instr == "READ") {
            uint8_t var = storeVar(tokens[1]);
            uint16_t address = std::stoi(tokens[2]);
            append(bytecode, static_cast<uint8_t>(OpCode::READ), {
                var,
                static_cast<uint8_t>(address & 0xFF), // lowbyte
                static_cast<uint8_t>(address >> 8) // highbyte
            });
        } else if (instr == "SLEEP") {
            uint8_t value = std::stoi(tokens[1]);
            append(bytecode, static_cast<uint8_t>(OpCode::SLEEP), {value});
        } else if (instr == "FOR") {
            if (loopRegisterCounter >= 3) {
                append(bytecode, static_cast<uint8_t>(OpCode::NOP), {});
                continue;
            }
            
            // This is the part that will be fixed.
            // =======================================================================
            // OLD, BUGGY CODE using stringstream and getline is removed.
            //
            // NEW, CORRECTED LOGIC that respects bracket nesting:
            
            std::string instructionListStr = tokens[1].substr(1, tokens[1].size() - 2);
            std::vector<std::string> nestedInstructions;
            std::string currentInstruction;
            int bracketLevel = 0;

            for (char c : instructionListStr) {
                if (c == '[') {
                    bracketLevel++;
                } else if (c == ']') {
                    bracketLevel--;
                }

                if (c == ';' && bracketLevel == 0) {
                    // Found a top-level instruction separator
                    // Trim leading/trailing whitespace
                    currentInstruction.erase(0, currentInstruction.find_first_not_of(" \t\r\n"));
                    currentInstruction.erase(currentInstruction.find_last_not_of(" \t\r\n") + 1);
                    if (!currentInstruction.empty()) {
                        nestedInstructions.push_back(currentInstruction);
                    }
                    currentInstruction.clear(); // Reset for the next instruction
                } else {
                    currentInstruction += c;
                }
            }
            // Add the last instruction in the list (which doesn't end with a ';')
            currentInstruction.erase(0, currentInstruction.find_first_not_of(" \t\r\n"));
            currentInstruction.erase(currentInstruction.find_last_not_of(" \t\r\n") + 1);
            if (!currentInstruction.empty()) {
                nestedInstructions.push_back(currentInstruction);
            }
            // =======================================================================

            uint8_t currentRegister = loopRegisterCounter++;
            // The stoi call will now receive a clean number string ("10", "20", etc.)
            uint8_t repeat = std::stoi(tokens[2]);
            
            // Emit MOV instruction to set the loop counter
            append(bytecode, static_cast<uint8_t>(OpCode::MOV), {currentRegister, repeat});

            // Note the start address for the jump
            uint16_t loopStartAddress = bytecode.size();

            // Emit JMP_ZERO to jump at the end of the loop if repeat is done
            append(bytecode, static_cast<uint8_t>(OpCode::JMP_ZERO), {
                currentRegister,
                static_cast<uint8_t>(0), // reserve lowbyte of address
                static_cast<uint8_t>(0)  // reserve highbyte of address
            });

            // Recursively compile the correctly parsed instructions
            compileInstructions(nestedInstructions, bytecode);

            // Emit JMP to jump back to the start of the loop
            append(bytecode, static_cast<uint8_t>(OpCode::JMP), {
                static_cast<uint8_t>(loopStartAddress & 0xFF), // lowbyte of address
                static_cast<uint8_t>(loopStartAddress >> 8)    // highbyte of address
            });

            // Record the end address to terminate the loop
            uint16_t loopEndAddress = bytecode.size();
            bytecode[(int)loopStartAddress + 2] = static_cast<uint8_t>(loopEndAddress & 0xFF); // Update lowbyte of address
            bytecode[(int)loopStartAddress + 3] = static_cast<uint8_t>(loopEndAddress >> 8); // Update highbyte of address

            loopRegisterCounter--; // Free up the register
        }
    }   
}
