#include <iostream>
#include <fstream>
#include <vector>

struct DumbNode
{
    std::string name;
    std::string left;
    std::string right;
};

struct Node
{
    std::string name;
    Node* left;
    Node* right;
};

int main()
{
    std::ifstream file;
    file.open("../data/day08.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::string instructions;
    getline(file, instructions);

    std::vector<DumbNode> dumbNodes;
    std::string line;
    getline(file, line);    // skip blank line.
    while (getline(file, line))
    {
        std::string nodeName = line.substr(0, 3);
        std::string leftName = line.substr(7, 3);
        std::string rightName = line.substr(12, 3);
        dumbNodes.push_back(DumbNode { .name = nodeName, .left = leftName, .right = rightName });
    }

    file.close();

    // turn dumb nodes into smart node network.

    // 1. build collection with disconnected left/rights.
    std::vector<Node> nodes;
    for (auto& dumbNode : dumbNodes)
        nodes.push_back(Node { .name = dumbNode.name, .left = NULL, .right = NULL });

    // 2. connect left/rights.
    for (auto& dumbNode : dumbNodes)
    {
        Node* target;
        Node* leftTarget;
        Node* rightTarget;
        // find the smart nodes for the node and its left and right.
        for (auto& node : nodes)
        {
            if (node.name == dumbNode.name)
                target = &node;
            if (node.name == dumbNode.left)
                leftTarget = &node;
            if (node.name == dumbNode.right)
                rightTarget = &node;
        }
        // connect 'em.
        target->left = leftTarget;
        target->right = rightTarget;
    }

    Node* AAA;
    for (auto& node : nodes)
        if (node.name == "AAA")
        {
            AAA = &node;
            break;
        }

    Node* ptr = AAA;
    long steps = 0L;
    while (ptr->name != "ZZZ")
    {
        char dir = instructions[steps++ % instructions.length()];
        ptr = dir == 'L' ? ptr->left : ptr->right;
    }

    std::cout << "steps = " << steps << std::endl;
}
