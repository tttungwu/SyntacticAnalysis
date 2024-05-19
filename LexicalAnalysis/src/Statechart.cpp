//
// Created by Tung Wu on 2024/5/19.
//

#include "../include/Statechart.h"

Statechart::Statechart()
{
    for (int i = 0; i < 7; ++i)
    {
        diagram[i] = std::map<std::pair<int, char>, int>();
        finalState[i] = std::map<int, bool>();
    }

    // IDN
    for (char c = 'a'; c <= 'z'; ++ c)
    {
        diagram[IDN][std::make_pair(0, c)] = 1;
        diagram[IDN][std::make_pair(0, c + 'A' - 'a')] = 1;
    }
    for (char c = 'a'; c <= 'z'; ++ c)
    {
        diagram[IDN][std::make_pair(1, c)] = 1;
        diagram[IDN][std::make_pair(1, c + 'A' - 'a')] = 1;
    }
    for (char c = '0'; c <= '9'; ++ c)
        diagram[IDN][std::make_pair(1, c)] = 1;
    diagram[IDN][std::make_pair(1, '.')] = 2;
    diagram[IDN][std::make_pair(1, '_')] = 2;
    for (char c = 'a'; c <= 'z'; ++ c)
    {
        diagram[IDN][std::make_pair(2, c)] = 2;
        diagram[IDN][std::make_pair(2, c + 'A' - 'a')] = 2;
    }
    for (char c = '0'; c <= '9'; ++ c)
        diagram[IDN][std::make_pair(2, c)] = 2;

    finalState[IDN][2] = true;
    finalState[IDN][1] = true;



    // SIGN
    diagram[SIGN][std::make_pair(0, '+')] = 1;
    diagram[SIGN][std::make_pair(0, '-')] = 1;
    diagram[SIGN][std::make_pair(0, '*')] = 1;
    diagram[SIGN][std::make_pair(0, '/')] = 1;
    diagram[SIGN][std::make_pair(0, '<')] = 1;
    diagram[SIGN][std::make_pair(0, '>')] = 1;
    diagram[SIGN][std::make_pair(0, '=')] = 1;
    diagram[SIGN][std::make_pair(0, '(')] = 1;
    diagram[SIGN][std::make_pair(0, ')')] = 1;

    finalState[SIGN][1] = true;



    // PERIOD
    diagram[PERIOD][std::make_pair(0, ';')] = 1;

    finalState[PERIOD][1] = true;



    // KEYWORDS
    diagram[KEYWORDS][std::make_pair(0, 'i')] = 1;
    diagram[KEYWORDS][std::make_pair(1, 'f')] = 2;
    diagram[KEYWORDS][std::make_pair(0, 't')] = 3;
    diagram[KEYWORDS][std::make_pair(3, 'h')] = 4;
    diagram[KEYWORDS][std::make_pair(4, 'e')] = 5;
    diagram[KEYWORDS][std::make_pair(5, 'n')] = 6;
    diagram[KEYWORDS][std::make_pair(0, 'e')] = 7;
    diagram[KEYWORDS][std::make_pair(7, 'l')] = 8;
    diagram[KEYWORDS][std::make_pair(8, 's')] = 9;
    diagram[KEYWORDS][std::make_pair(9, 'e')] = 10;
    diagram[KEYWORDS][std::make_pair(0, 'w')] = 11;
    diagram[KEYWORDS][std::make_pair(11, 'h')] = 12;
    diagram[KEYWORDS][std::make_pair(12, 'i')] = 13;
    diagram[KEYWORDS][std::make_pair(13, 'l')] = 14;
    diagram[KEYWORDS][std::make_pair(14, 'e')] = 15;
    diagram[KEYWORDS][std::make_pair(0, 'd')] = 16;
    diagram[KEYWORDS][std::make_pair(16, 'o')] = 17;

    finalState[KEYWORDS][2] = true;
    finalState[KEYWORDS][6] = true;
    finalState[KEYWORDS][10] = true;
    finalState[KEYWORDS][15] = true;
    finalState[KEYWORDS][17] = true;



    // DECIMAL
    diagram[DECIMAL][std::make_pair(0, '0')] = 2;
    diagram[DECIMAL][std::make_pair(1, '0')] = 1;
    for (char c = '1'; c <= '9'; ++ c)
    {
        diagram[DECIMAL][std::make_pair(0, c)] = 1;
        diagram[DECIMAL][std::make_pair(1, c)] = 1;
    }
    diagram[DECIMAL][std::make_pair(1, '.')] = 3;
    diagram[DECIMAL][std::make_pair(2, '.')] = 3;
    for (char c = '0'; c <= '9'; ++ c)
    {
        diagram[DECIMAL][std::make_pair(3, c)] = 4;
        diagram[DECIMAL][std::make_pair(4, c)] = 4;
    }

    finalState[DECIMAL][1] = true;
    finalState[DECIMAL][2] = true;
    finalState[DECIMAL][4] = true;



    // OCTAL
    diagram[OCTAL][std::make_pair(0, '0')] = 1;
    diagram[OCTAL][std::make_pair(1, 'o')] = 2;
    diagram[OCTAL][std::make_pair(1, 'O')] = 2;
    for (char c = '0'; c <= '7'; ++ c)
    {
        diagram[OCTAL][std::make_pair(2, c)] = 3;
        diagram[OCTAL][std::make_pair(3, c)] = 3;
    }
    diagram[OCTAL][std::make_pair(3, '.')] = 4;
    for (char c = '0'; c <= '7'; ++ c)
    {
        diagram[OCTAL][std::make_pair(4, c)] = 5;
        diagram[OCTAL][std::make_pair(5, c)] = 5;
    }

    finalState[OCTAL][3] = true;
    finalState[OCTAL][5] = true;



    // HEXADECIMAL
    diagram[HEXADECIMAL][std::make_pair(0, '0')] = 1;
    diagram[HEXADECIMAL][std::make_pair(1, 'x')] = 2;
    diagram[HEXADECIMAL][std::make_pair(1, 'X')] = 2;
    for (char c = '0'; c <= '9'; ++ c)
    {
        diagram[HEXADECIMAL][std::make_pair(2, c)] = 3;
        diagram[HEXADECIMAL][std::make_pair(3, c)] = 3;
    }
    for (char c = 'a'; c <= 'f'; ++ c)
    {
        diagram[HEXADECIMAL][std::make_pair(2, c)] = 3;
        diagram[HEXADECIMAL][std::make_pair(3, c)] = 3;
    }
    diagram[HEXADECIMAL][std::make_pair(3, '.')] = 4;
    for (char c = '0'; c <= '9'; ++ c)
    {
        diagram[HEXADECIMAL][std::make_pair(4, c)] = 5;
        diagram[HEXADECIMAL][std::make_pair(5, c)] = 5;
    }
    for (char c = 'a'; c <= 'f'; ++ c)
    {
        diagram[HEXADECIMAL][std::make_pair(4, c)] = 5;
        diagram[HEXADECIMAL][std::make_pair(5, c)] = 5;
    }

    finalState[HEXADECIMAL][3] = true;
    finalState[HEXADECIMAL][5] = true;
}

int Statechart::transferTo(std::map<std::pair<int, char>, int> &diagram, int state, char c)
{
    if (diagram.count(std::make_pair(state, c)) == 0) return -1;
    return diagram[std::make_pair(state, c)];
}

bool Statechart::isFinalState(std::map<int, bool> &finalState, int state)
{
    return finalState.count(state);
}

std::vector<Token> Statechart::getToken(std::string line)
{
    std::vector<Token> res;
    for (int i = 0; i < line.size(); ++ i)
    {
        bool bad = true;
        for (int j = static_cast<int>(Type::KEYWORDS); j <= static_cast<int>(Type::DECIMAL); j ++)
        {
            Type type = static_cast<Type>(j);
            std::string value = "";
            std::map<std::pair<int, char>, int> &diagram = this->diagram[type];
            std::map<int, bool> &finalState = this->finalState[type];
            int state = 0;
            for (int k = i; k < line.size(); ++ k)
            {
                int to = transferTo(diagram, state, line[k]);
                if (to == -1)
                {
                    if (isFinalState(finalState, state))
                    {
                        bad = false;
                        i = k - 1;
                    }
                    break;
                }
                value += line[k];
                state = to;
                if (k == line.size() - 1 && isFinalState(finalState, state))
                {
                    bad = false;
                    i = k;
                }
            }
            if (!bad)
            {
                res.emplace_back(value, type);
                break;
            }
        }
        if (bad)
        {
            res.clear();
            break;
        }
    }
    return res;
}
