#include "SegmentNode.h"

class TreePrinter {

private:

    bool printToConsole(const std::string& prefix, const SegmentNode* node, bool isLeft)
    {
        if (node != nullptr)
        {
            std::cout << prefix;

            std::cout << (isLeft ? "|--" : "|__");

            // print the value of the node
            std::cout << "[" << node->range.a << ", " << node->range.b << "]" << std::endl;

            // enter the next tree level - left and right branch
            if (node->isLThread == false)
                printToConsole(prefix + (isLeft ? "|   " : "    "), node->lLink, true);
            if(node->isRThread == false)
                printToConsole(prefix + (isLeft ? "|   " : "    "), node->rLink, false);
        }
        return true;
    }

public:

    bool printToConsole(const SegmentNode* node)
    {
        return printToConsole("", node, false);
    };
};