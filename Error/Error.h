//
// Created by Tung Wu on 2024/5/19.
//

#include <string>

#ifndef CODE_ERROR_H
#define CODE_ERROR_H

enum class ErrorType {
    LexicalError,
    SyntaxError,
    SemanticError,
    CodeGenError,
    OtherError,
};


class Error {
public:
    static void printErrors(ErrorType type, const std::string &message, bool exitFlag = true, int line = -1);

private:
    static std::string getTypeString(ErrorType type);
};


#endif //CODE_ERROR_H
