//
// Created by Tung Wu on 2024/5/19.
//

#ifndef CODE_LEXER_H
#define CODE_LEXER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Token.h"
#include "../../Error/Error.h"
#include "Statechart.h"


class Lexer {
public:
    static std::vector<Token> tokenize(const std::string &filename);

private:
    static std::vector<Token> scanLine(std::string line);
};


#endif //CODE_LEXER_H
