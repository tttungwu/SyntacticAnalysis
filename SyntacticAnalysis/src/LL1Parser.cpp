//
// Created by Tung Wu on 2024/5/19.
//

#include "../include/LL1Parser.h"


std::vector<Token> LL1Parser::tokens;
Token LL1Parser::cur_token = Token("", Type::EMPTY);
long long LL1Parser::token_idx = 0;
int LL1Parser::line_cnt = 0;

void LL1Parser::match(Type expectedType, std::string expectedValue)
{
    if (LL1Parser::cur_token.getType() == expectedType && (expectedValue.empty() || LL1Parser::cur_token.getValue() == expectedValue))
    {
        if (LL1Parser::token_idx < LL1Parser::tokens.size() - 1)
            LL1Parser::cur_token = LL1Parser::tokens[++ LL1Parser::token_idx];
        else
        {
            ++ LL1Parser::token_idx;
            LL1Parser::cur_token = Token("", Type::EMPTY);
            if (LL1Parser::token_idx > LL1Parser::tokens.size())
                Error::printErrors(ErrorType::SyntaxError, "match: Missing tokens", true, LL1Parser::line_cnt);
        }
    }
    else Error::printErrors(ErrorType::SyntaxError, "match: Ungrammatical", true, LL1Parser::line_cnt);
}

void LL1Parser::parse(std::vector<Token> tokens)
{
    if (tokens.empty()) return;

    // 初始化数据
    LL1Parser::tokens = tokens;
    LL1Parser::token_idx = 0;
    LL1Parser::cur_token = LL1Parser::tokens[LL1Parser::token_idx];
    LL1Parser::line_cnt = 0;

    // 进行语法分析
#ifndef USE_VISUALIZATION
    LL1Parser::parseP();
#else
    NTree* root = LL1Parser::parseP();
    std::vector<bool> flag(30, true);
    NTree::printNTree(root, flag);
#endif
}

ReturnType LL1Parser::parseP()
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseK();
#else
    NTree* root = new NTree("S");
    NTree* son = parseK();
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseK()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getType() == Type::EMPTY) return;
    LL1Parser::parseS();
    LL1Parser::match(Type::PERIOD, ";");
    ++ LL1Parser::line_cnt;
    LL1Parser::parseK();
#else
    NTree* root = new NTree("K"), *son;
    if (LL1Parser::cur_token.getType() == Type::EMPTY)
    {
        son = new NTree("ε");
        root->append(son);
    }
    else
    {
        son = LL1Parser::parseS();
        root->append(son);
        LL1Parser::match(Type::PERIOD, ";");
        son = new NTree(";");
        root->append(son);
        ++ LL1Parser::line_cnt;
        son = LL1Parser::parseK();
        root->append(son);
    }
    return root;
#endif
}

ReturnType LL1Parser::parseS()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        LL1Parser::match(Type::IDN);
        LL1Parser::match(Type::SIGN, "=");
        parseE();
    }
    else if (LL1Parser::cur_token.getValue() == "if")
    {
        LL1Parser::match(Type::KEYWORDS, "if");
        parseC();
        LL1Parser::match(Type::KEYWORDS, "then");
        parseS();
        parseA();
    }
    else if (LL1Parser::cur_token.getValue() == "while")
    {
        LL1Parser::match(Type::KEYWORDS, "while");
        LL1Parser::parseC();
        LL1Parser::match(Type::KEYWORDS, "do");
        LL1Parser::parseS();
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseS: Ungrammatical", true,LL1Parser::line_cnt);
#else
    NTree* root = new NTree("S"), *son;
    if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::IDN);
        son = new NTree(LL1Parser::cur_token.getValue());
        LL1Parser::match(Type::SIGN, "=");
        son = parseE();
        root->append(son);
    }
    else if (LL1Parser::cur_token.getValue() == "if")
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "if");
        son = parseC();
        root->append(son);
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "then");
        son = parseS();
        root->append(son);
        son = parseA();
        root->append(son);
    }
    else if (LL1Parser::cur_token.getValue() == "while")
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "while");
        son = LL1Parser::parseC();
        root->append(son);
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "do");
        son = LL1Parser::parseS();
        root->append(son);
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseS: Ungrammatical", true,LL1Parser::line_cnt);
    return root;
#endif
}

ReturnType LL1Parser::parseA()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "else")
    {
        LL1Parser::match(Type::KEYWORDS, "else");
        parseS();
    }
    else return;
#else
    NTree *root = new NTree("A"), *son;
    if (LL1Parser::cur_token.getValue() == "else")
    {
        son = new NTree("else");
        root->append(son);
        LL1Parser::match(Type::KEYWORDS, "else");
        son = parseS();
        root->append(son);
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
    }
    return root;
#endif
}

ReturnType LL1Parser::parseC()
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseE();
    LL1Parser::parseB();
#else
    NTree* root = new NTree("C"), *son;
    son = LL1Parser::parseE();
    root->append(son);
    son = LL1Parser::parseB();
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseB()
{
    if (LL1Parser::cur_token.getValue() == "=" || LL1Parser::cur_token.getValue() == "<" || LL1Parser::cur_token.getValue() == ">")
    {
#ifndef USE_VISUALIZATION
        LL1Parser::match(Type::SIGN);
        parseE();
#else
        NTree* root = new NTree("B"), *son;
        son = new NTree(LL1Parser::cur_token.getValue());
        LL1Parser::match(Type::SIGN);
        root->append(son);
        son = parseE();
        root->append(son);
        return root;
#endif
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseB: Ungrammatical", true,LL1Parser::line_cnt);
}

ReturnType LL1Parser::parseE()
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseT();
    LL1Parser::parseD();
#else
    NTree* root = new NTree("E"), *son;
    son = LL1Parser::parseT();
    root->append(son);
    son = LL1Parser::parseD();
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseD()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "-" || LL1Parser::cur_token.getValue() == "+")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseT();
        LL1Parser::parseD();
    }
    else return;
#else
    NTree* root = new NTree("D"), *son;
    if (LL1Parser::cur_token.getValue() == "-" || LL1Parser::cur_token.getValue() == "+")
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        son = LL1Parser::parseT();
        root->append(son);
        son = LL1Parser::parseD();
        root->append(son);
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
    }
    return root;
#endif
}

ReturnType LL1Parser::parseT()
{
#ifndef USE_VISUALIZATION
    LL1Parser::parseF();
    LL1Parser::parseG();
#else
    NTree* root = new NTree("T"), *son;
    son = LL1Parser::parseF();
    root->append(son);
    son = LL1Parser::parseG();
    root->append(son);
    return root;
#endif
}

ReturnType LL1Parser::parseG()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "*" || LL1Parser::cur_token.getValue() == "/")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseF();
        LL1Parser::parseG();
    }
    else return;
#else
    NTree* root = new NTree("G"), *son;
    if (LL1Parser::cur_token.getValue() == "*" || LL1Parser::cur_token.getValue() == "/")
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        son = LL1Parser::parseF();
        root->append(son);
        son = LL1Parser::parseG();
        root->append(son);
    }
    else
    {
        son = new NTree("ε");
        root->append(son);
    }
    return root;
#endif
}

ReturnType LL1Parser::parseF()
{
#ifndef USE_VISUALIZATION
    if (LL1Parser::cur_token.getValue() == "(")
    {
        LL1Parser::match(Type::SIGN, "(");
        LL1Parser::parseE();
        LL1Parser::match(Type::SIGN, ")");
    }
    else if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        LL1Parser::match(Type::IDN);
    }
    else if (LL1Parser::cur_token.getType() == Type::OCTAL)
    {
        LL1Parser::match(Type::OCTAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::DECIMAL)
    {
        LL1Parser::match(Type::DECIMAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::HEXADECIMAL)
    {
        LL1Parser::match(Type::HEXADECIMAL);
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseF: Ungrammatical", true,LL1Parser::line_cnt);
#else
    NTree* root = new NTree("F"), *son;
    if (LL1Parser::cur_token.getValue() == "(")
    {
        son = new NTree("(");
        root->append(son);
        LL1Parser::match(Type::SIGN, "(");
        son = LL1Parser::parseE();
        root->append(son);
        son = new NTree(")");
        root->append(son);
        LL1Parser::match(Type::SIGN, ")");
    }
    else if (LL1Parser::cur_token.getType() == Type::IDN)
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::IDN);
    }
    else if (LL1Parser::cur_token.getType() == Type::OCTAL)
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::OCTAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::DECIMAL)
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::DECIMAL);
    }
    else if (LL1Parser::cur_token.getType() == Type::HEXADECIMAL)
    {
        son = new NTree(LL1Parser::cur_token.getValue());
        root->append(son);
        LL1Parser::match(Type::HEXADECIMAL);
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseF: Ungrammatical", true,LL1Parser::line_cnt);
    return root;
#endif

}