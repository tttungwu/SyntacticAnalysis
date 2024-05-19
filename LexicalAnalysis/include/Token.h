//
// Created by Tung Wu on 2024/5/19.
//

#include <string>
#include <iostream>
#include "Type.h"

#ifndef CODE_TOKEN_H
#define CODE_TOKEN_H


class Token {
private:
    std::string value;
    Type type;

public:
    Token(std::string, Type);
    std::string getValue();
    Type getType();
    void print();
};


#endif //CODE_TOKEN_H
