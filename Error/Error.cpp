//
// Created by Tung Wu on 2024/5/19.
//

#include "Error.h"
#include <iostream>


std::string Error::getTypeString(ErrorType type) {
    switch (type) {
        case ErrorType::LexicalError: return "Lexical Error";
        case ErrorType::SyntaxError: return "Syntax Error";
        case ErrorType::SemanticError: return "Semantic Error";
        case ErrorType::CodeGenError: return "Code Generation Error";
        default: return "Unknown Error";
    }
}

void Error::printErrors(ErrorType type, const std::string &message, bool exitFlag, int line) {
    std::cout << "Error Type: " << getTypeString(type) << "\n";
    if (line != -1) std::cout << "Line: " << line << "\n";
    std::cout << "Message: " << message << "\n\n";
    if (exitFlag) exit(1);
}