//
// Created by Tung Wu on 2024/5/19.
//

#ifndef CODE_NTREE_H
#define CODE_NTREE_H

#include <iostream>
#include <string>
#include <vector>


class NTree {
    NTree(){};
    NTree(std::string val) : val(val){}

    static void printNTree(NTree* x, std::vector<bool> flag, int depth = 0, bool isLast = false);


private:
    std::string val;
    std::vector<NTree*> child;
};


#endif //CODE_NTREE_H
