#include "Node.h"

class TreePrinter {

private:

    bool printToConsole(const std::string& prefix, const Node* node, bool isLeft)
    {
        if (node != nullptr)
        {
            std::cout << prefix;

            std::cout << (isLeft ? "|--" : "|__");

            // print the value of the node
            std::cout << node->k << std::endl;

            // enter the next tree level - left and right branch
            if (node->isLThread == false)
                printToConsole(prefix + (isLeft ? "|   " : "    "), node->lLink, true);
            if(node->isRThread == false)
                printToConsole(prefix + (isLeft ? "|   " : "    "), node->rLink, false);
        }
        return true;
    }

public:

    bool printToConsole(const Node* node)
    {
        return printToConsole("", node, false);
    };
};