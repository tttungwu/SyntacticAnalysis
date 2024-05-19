//
// Created by Tung Wu on 2024/5/19.
//

#include <map>
#include "../include/Token.h"

Token::Token(std::string v, Type t) : value(v), type(t) {}

std::string Token::getValue()
{
    return this->value;
}

Type Token::getType()
{
    return this->type;
}

void Token::print()
{
    static std::map<Type, std::string> typeNames = {
            {Type::KEYWORDS, "KEYWORDS"},
            {Type::SIGN, "SIGN"},
            {Type::PERIOD, "PERIOD"},
            {Type::IDN, "IDN"},
            {Type::OCTAL, "OCTAL"},
            {Type::HEXADECIMAL, "HEXADECIMAL"},
            {Type::DECIMAL, "DECIMAL"}
    };
    std::cout << typeNames[this->type] << ' ' << this->value << std::endl;
}
