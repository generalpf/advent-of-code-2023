#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

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

    std::vector<Node*> ptrs;
    for (auto& node : nodes)
        if (node.name[2] == 'A')
            ptrs.push_back(&node);

    long steps = 0L;
    struct ZInfo
    {
        long firstZStep = -1;
        std::string firstZName;
        long zDistance = -1;
    };
    std::vector<ZInfo> zInfo;
    for (size_t i = 0; i < ptrs.size(); i++)
        zInfo.push_back(ZInfo());
    while (true)
    {
        if (steps % instructions.length() == 0)
        {
            bool haveSeenAllDistances = true;
            for (int i = 0; i < ptrs.size(); i++)
            {
                if (ptrs[i]->name[2] == 'Z')
                {
                    if (zInfo[i].firstZStep == -1)
                    {
                        zInfo[i].firstZStep = steps;
                        zInfo[i].firstZName = ptrs[i]->name;
                    }
                    else if (zInfo[i].zDistance == -1 && ptrs[i]->name == zInfo[i].firstZName)
                        zInfo[i].zDistance = steps - zInfo[i].firstZStep;
                }
                if (zInfo[i].zDistance == -1)
                    haveSeenAllDistances = false;
            }
            if (haveSeenAllDistances) break;
        }
        char dir = instructions[steps++ % instructions.length()];
        for (int i = 0; i < ptrs.size(); i++)
            ptrs[i] = dir == 'L' ? ptrs[i]->left : ptrs[i]->right;
    }

    long answer = std::lcm(zInfo[0].zDistance, zInfo[1].zDistance);
    for (size_t i = 1; i < zInfo.size() - 1; i++)
        answer = std::lcm(answer, zInfo[i + 1].zDistance);

    std::cout << "answer = " << answer << std::endl;
}
