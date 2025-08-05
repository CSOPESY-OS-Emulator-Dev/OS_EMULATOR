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

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (c == '\\') {
            // This is an escape character.
            // Check if there is a character after it to escape.
            if (i + 1 < input.length()) {
                // Append the *next* character to the token literally,
                // regardless of what it is (e.g., \", \\, \n).
                currentToken += input[i + 1];
                // Increment i to skip the next character in the loop,
                // since we've already processed it.
                i++;
            }
            // If a backslash is the very last character, it's ignored.
        } else if (c == '"') {
            // This is a non-escaped quote. Toggle the state.
            inQuotes = !inQuotes;
            // Append the quote to the token, as requested.
            currentToken += c;
        } else if (std::isspace(c) && !inQuotes) {
            // A space outside of quotes is a delimiter.
            if (!currentToken.empty()) {
                allTokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            // This is a standard character, append it.
            currentToken += c;
        }
    }

    // Add the very last token in the string if it exists.
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