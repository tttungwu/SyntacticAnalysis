//
// Created by Tung Wu on 2024/5/19.
//

#ifndef CODE_LL1PARSER_H
#define CODE_LL1PARSER_H

#include "Parser.h"
#include "../../Error/Error.h"


class LL1Parser : public Parser {
public:
    static void parse(std::vector<Token>);

private:
    static std::vector<Token> tokens;   // 等待处理的所有token
    static Token cur_token;    // 当前将处理的token
    static long long token_idx;     // 当前将处理的token在所有token中的下标
    static int line_cnt;      // 当前处理的行

    // 分别处理不同的非终结符
    static void parseP();
    static void parseK();
    static void parseS();
    static void parseA();
    static void parseC();
    static void parseE();
    static void parseB();
    static void parseT();
    static void parseD();
    static void parseF();
    static void parseG();

    static void match(Type expectedType, std::string expectedValue = "");    // 处理当前token并找到下一个token
};


#endif //CODE_LL1PARSER_H
