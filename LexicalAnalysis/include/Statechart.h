//
// Created by Tung Wu on 2024/5/19.
//

#ifndef CODE_STATECHART_H
#define CODE_STATECHART_H

#include <map>
#include <vector>
#include <string>
#include "Token.h"
#include "Type.h"


class Statechart {
private:
    // 对应7种不同类型的状态图
    std::map<std::pair<int, char>, int> diagram[7];
    // 对应7种不同类型的状态图的终结态
    std::map<int, bool> finalState[7];

    // 返回对应状态的转移结果，如果返回-1表示转移失败
    int transferTo(std::map<std::pair<int, char>, int> &diagram, int state, char c);
    // 判断对应状态是否终结态
    bool isFinalState(std::map<int, bool> &finalState, int state);
public:
    Statechart();
    // 从一个不含有空格的字符串中分割出token，如果str非空且返回的vector为空，则说明无法合法分割
    std::vector<Token> getToken(std::string line);
};


#endif //CODE_STATECHART_H
