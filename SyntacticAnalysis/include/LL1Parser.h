//
// Created by Tung Wu on 2024/5/19.
//

#ifndef CODE_LL1PARSER_H
#define CODE_LL1PARSER_H

#include "Parser.h"
#include "../../Error/Error.h"
#include "../Visualization/NTree.h"
#include "../Symbol/NonTerminal.h"

#define USE_VISUALIZATION

#ifdef USE_VISUALIZATION
using ReturnType = NTree*;
#else
using ReturnType = void;
#endif


class LL1Parser : public Parser {
public:
    static void parse(std::vector<Token>);

private:
    static std::vector<Token> tokens;   // 等待处理的所有token
    static Token cur_token;    // 当前将处理的token
    static long long token_idx;     // 当前将处理的token在所有token中的下标
    static int line_cnt;      // 当前处理的行

    // 分别处理不同的非终结符
    static ReturnType parseP(P *p = nullptr);
    static ReturnType parseK(K *k = nullptr);
    static ReturnType parseS(S *s = nullptr);
    static ReturnType parseA(A *a = nullptr);
    static ReturnType parseC(C *c = nullptr);
    static ReturnType parseE(E *e = nullptr);
    static ReturnType parseB(B *b = nullptr);
    static ReturnType parseT(T *t = nullptr);
    static ReturnType parseD(D *d = nullptr);
    static ReturnType parseF(F *f = nullptr);
    static ReturnType parseG(G *G = nullptr);

    static void match(Type expectedType, std::string expectedValue = "");    // 处理当前token并找到下一个token
};


#endif //CODE_LL1PARSER_H
