//
// Created by Tung Wu on 2024/5/20.
//

#include "NonTerminal.h"

std::string NonTerminal::newlabel()
{
    static int cnt = 0;
    return "L" + std::to_string(cnt ++);
}

std::string NonTerminal::newtemp()
{
    static int cnt = 0;
    return "t" + std::to_string(cnt ++);
}