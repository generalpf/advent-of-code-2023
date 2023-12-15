#include <iostream>
#include <fstream>
#include <vector>

unsigned char HASH(std::string s);

struct LabelAndLens
{
    std::string label;
    unsigned char lens;
};

int main()
{
    std::ifstream file;
    file.open("../data/day15.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::string line;
    getline(file, line);
    file.close();

    std::vector<LabelAndLens> HASHMAP[256];

    while (true)
    {
        // pull out next instruction.
        size_t nextComma = line.find_first_of(",");
        std::string instruction = (nextComma == std::string::npos ? line : line.substr(0, nextComma));
        
        // parse instruction.
        size_t opIndex = instruction.find_first_of("=-");
        std::string label = instruction.substr(0, opIndex);
        unsigned char labelHash = HASH(label);
        char op = instruction[opIndex];

        // execute = instruction.
        if (op == '=')
        {
            unsigned char lens = instruction[opIndex + 1] - '0';
            bool replaced = false;
            for (std::vector<LabelAndLens>::iterator ll = HASHMAP[labelHash].begin(); ll != HASHMAP[labelHash].end(); ll++)
                if (ll->label == label)
                {
                    ll->lens = lens;
                    replaced = true;
                    break;
                }
            if (!replaced)
                HASHMAP[labelHash].push_back(LabelAndLens { .label = label, .lens = lens });
        }
        // execute - instruction.
        else 
        {
            for (std::vector<LabelAndLens>::iterator ll = HASHMAP[labelHash].begin(); ll != HASHMAP[labelHash].end(); ll++)
                if (ll->label == label)
                {
                    HASHMAP[labelHash].erase(ll);
                    break;
                }
        }

        // advance instruction pointer.
        if (nextComma == std::string::npos) break;
        line = line.substr(nextComma + 1);
    }

    long power = 0L;
    
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < HASHMAP[i].size(); j++)
            power += (i + 1) * (j + 1) * HASHMAP[i][j].lens;

    std::cout << "focusing power = " << power << std::endl;
}

unsigned char HASH(std::string s)
{
    long h = 0L;
    for (size_t i = 0; i < s.length(); i++)
    {
        h += s[i];
        h *= 17;
        h %= 256;
    }
    return h;
}
