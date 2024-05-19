//
// Created by Tung Wu on 2024/5/19.
//

#include "../include/Lexer.h"


std::vector<Token> Lexer::scanLine(std::string line)
{
    static int cntLine = 1;
    static Statechart statechart;

    std::deque<std::string> words;
    std::stringstream ssin(line);
    std::string word;
    while (ssin >> word) words.push_back(word);

    std::vector<Token> tokens;
    for (auto s : words)
    {
        std::vector<Token> split = statechart.getToken(s);
        if (split.empty()) Error::printErrors(ErrorType::LexicalError, "Unexpected character", true, cntLine ++);
        for (auto tmp : split)
            tokens.push_back(tmp);
    }

    return tokens;
}

std::vector<Token> Lexer::tokenize(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) Error::printErrors(ErrorType::OtherError, "Could not open the file \"" + filename + "\".");

    std::vector<Token> tokens;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<Token> lineTokens = Lexer::scanLine(line);
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }

    return tokens;
}
