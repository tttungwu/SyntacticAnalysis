#include <iostream>
#include <vector>
#include "LexicalAnalysis/include/Lexer.h"

int main()
{
//    Error::addError(ErrorType::LexicalError, "Unexpected character", 1, 5);
//    Error::addError(ErrorType::SyntaxError, "Missing semicolon", 2, 10);
//    Error::addError(ErrorType::SemanticError, "Undeclared variable", 3, 15);
//    Error::addError(ErrorType::CodeGenError, "Invalid opcode", 4, 20);

    std::vector<std::vector<Token>> res = Lexer::tokenize("../test.txt");
}