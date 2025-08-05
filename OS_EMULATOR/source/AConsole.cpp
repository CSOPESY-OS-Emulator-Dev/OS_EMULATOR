#include "AConsole.h"

AConsole::AConsole(std::string name) {
	this->name = name;
}

/**
 * @brief Parses a command line string, preserving the enclosing quotes on arguments.
 *
 * This function implements a state machine to parse a command string. It treats
 * spaces as delimiters unless they are inside a pair of double quotes.
 * Unlike a standard shell parser, this version INCLUDES the quote characters
 * in the final argument token.
 *
 * @param input The raw command line string.
 *        Example: screen -s p1 "\"some instructions\""
 * @return A ParsedCommand struct containing the command and a vector of its arguments.
 */
ParsedCommand AConsole::parseInput(const std::string& input) {
    ParsedCommand result;
    std::vector<std::string> allTokens;
    std::string currentToken;
    bool inQuotes = false;

    for (char c : input) {
        if (c == '"') {
            // A quote simply toggles the state. The character itself will be
            // appended to the current token in the 'else' block below.
            inQuotes = !inQuotes;
        }

        if (std::isspace(c) && !inQuotes) {
            // A space outside of quotes acts as a delimiter.
            // Push the completed token and reset.
            if (!currentToken.empty()) {
                allTokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            // If the character is not a delimiter (i.e., it's a regular char,
            // a quote char, or a space inside quotes), append it.
            currentToken += c;
        }
    }

    // Add the very last token in the string, if it exists.
    if (!currentToken.empty()) {
        allTokens.push_back(currentToken);
    }

    // The first token is the command, the rest are arguments.
    if (!allTokens.empty()) {
        result.command = allTokens[0];
        if (allTokens.size() > 1) {
            result.args.assign(allTokens.begin() + 1, allTokens.end());
        }
    }

    return result;
}