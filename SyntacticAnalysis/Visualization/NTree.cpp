//
// Created by Tung Wu on 2024/5/19.
//

#include "NTree.h"

void NTree::printNTree(NTree* x, std::vector<bool> flag, int depth, bool isLast)
{
    // Condition when node is None
    if (x == NULL)
        return;

    // Loop to print the depths of the
    // current node
    for (int i = 1; i < depth; ++i) {

        // Condition when the depth
        // is exploring
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

    // Condition when the current
    // node is the root node
    if (depth == 0)
        std::cout << x->val << '\n';

        // Condition when the node is
        // the last node of
        // the exploring depth
    else if (isLast) {
        std::cout << "+--- " << x->val << '\n';

        // No more childrens turn it
        // to the non-exploring depth
        flag[depth] = false;
    }
    else {
        std::cout << "+--- " << x->val << '\n';
    }

    int it = 0;
    for (auto i = x->child.begin(); i != x->child.end(); ++i, ++it)

        // Recursive call for the
        // children nodes
        printNTree(*i, flag, depth + 1,it == (x->child.size()) - 1);
    flag[depth] = true;
}