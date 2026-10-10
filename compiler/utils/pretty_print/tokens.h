#ifndef PRETTY_PRINT_TOKENS_H
#define PRETTY_PRINT_TOKENS_H

#include <iostream>
#include <vector>

#include "core/tokens.h"
#include "utils/ansi/ansi.h"

inline void printProgramTokens(const std::vector<Token>& tokens)
{

    std::cout << "\nprogram tokens:\n" << std::endl;
    std::cout << ansi::color::bright_black;
        
    for(Token t : tokens){
        std::cout << typeToString(t.type) << std::endl;
    }
    std::cout << ansi::color::reset;
}

#endif //PRETTY_PRINT_TOKENS_H