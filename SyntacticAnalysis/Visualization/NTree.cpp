//
// Created by Tung Wu on 2024/5/19.
//

#include "NTree.h"

void NTree::printNTree(NTree* x, std::vector<bool> flag, int depth, bool isLast)
{
    if (x == NULL)
        return;
    for (int i = 1; i < depth; ++i) {
        if (flag[i] == true) {
            std::cout << "| "
                 << " "
                 << " "
                 << " ";
        }

            // Otherwise print
            // the blank spaces
        else {
            std::cout << " "
                 << " "
                 << " "
                 << " ";
        }
    }

    if (depth == 0)
        std::cout << x->val << '\n';

    else if (isLast) {
        std::cout << "+--- " << x->val << '\n';
        flag[depth] = false;
    }
    else {
        std::cout << "+--- " << x->val << '\n';
    }

    int it = 0;
    for (auto i = x->child.begin(); i != x->child.end(); ++i, ++it)
        printNTree(*i, flag, depth + 1,it == (x->child.size()) - 1);
    flag[depth] = true;
}

void NTree::append(NTree* son)
{
    if (son != nullptr) child.push_back(son);
}