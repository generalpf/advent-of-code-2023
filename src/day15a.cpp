#include <iostream>
#include <fstream>
#include <vector>

unsigned char HASH(std::string s);

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

    long sum = 0L;
    
    while (true)
    {
        size_t nextComma = line.find_first_of(",");
        std::string hashThis = (nextComma == std::string::npos ? line : line.substr(0, nextComma));
        long h = HASH(hashThis);
        sum += h;
        if (nextComma == std::string::npos) break;
        line = line.substr(nextComma + 1);
    }

    std::cout << "sum of hashes = " << sum << std::endl;
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