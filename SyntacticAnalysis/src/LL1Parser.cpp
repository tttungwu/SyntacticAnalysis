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
    LL1Parser::parseP();
}

void LL1Parser::parseP()
{
    LL1Parser::parseK();
}

void LL1Parser::parseK()
{
    if (LL1Parser::cur_token.getType() == Type::EMPTY) return;
    LL1Parser::parseS();
    LL1Parser::match(Type::PERIOD, ";");
    ++ LL1Parser::line_cnt;
    LL1Parser::parseK();
}

void LL1Parser::parseS()
{
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
}

void LL1Parser::parseA()
{
    if (LL1Parser::cur_token.getValue() == "else")
    {
        LL1Parser::match(Type::KEYWORDS, "else");
        parseS();
    }
    else return;
}

void LL1Parser::parseC()
{
    LL1Parser::parseE();
    LL1Parser::parseB();
}

void LL1Parser::parseB()
{
    if (LL1Parser::cur_token.getValue() == "=" || LL1Parser::cur_token.getValue() == "<" || LL1Parser::cur_token.getValue() == ">")
    {
        LL1Parser::match(Type::SIGN);
        parseE();
    }
    else Error::printErrors(ErrorType::SyntaxError, "parseB: Ungrammatical", true,LL1Parser::line_cnt);
}

void LL1Parser::parseE()
{
    LL1Parser::parseT();
    LL1Parser::parseD();
}

void LL1Parser::parseD()
{
    if (LL1Parser::cur_token.getValue() == "-" || LL1Parser::cur_token.getValue() == "+")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseT();
        LL1Parser::parseD();
    }
    else return;
}

void LL1Parser::parseT()
{
    LL1Parser::parseF();
    LL1Parser::parseG();
}

void LL1Parser::parseG()
{
    if (LL1Parser::cur_token.getValue() == "*" || LL1Parser::cur_token.getValue() == "/")
    {
        LL1Parser::match(Type::SIGN, LL1Parser::cur_token.getValue());
        LL1Parser::parseF();
        LL1Parser::parseG();
    }
    else return;
}

void LL1Parser::parseF()
{
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
}