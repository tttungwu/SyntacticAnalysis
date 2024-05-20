#include <iostream>
#include <vector>
#include "LexicalAnalysis/include/Lexer.h"
#include "SyntacticAnalysis/include/LL1Parser.h"

int main()
{
    std::vector<Token> res = Lexer::tokenize("../test.txt");
    LL1Parser::parse(res);

    return 0;
}