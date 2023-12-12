#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

long workingCombos(std::string s, std::regex r, size_t modAt);

int main()
{
    std::ifstream file;
    file.open("../data/day12.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    long sum = 0L;
    std::string line;
    while (getline(file, line))
    {
        size_t space = line.find_first_of(" ");
        std::string parts = line.substr(0, space);
        std::string reqs = line.substr(space + 1);
        std::string rs;
        while (true)
        {
            size_t nextComma = reqs.find_first_of(",");
            size_t until = (nextComma == std::string::npos) ? reqs.length() : nextComma;
            rs += "#{" + reqs.substr(0, until) + "}\\.+";
            if (nextComma == std::string::npos) break;
            reqs = reqs.substr(nextComma + 1);
        }
        // drop the trailing "\.+".
        rs = rs.substr(0, rs.length() - 3);
        // add starting and end anchors and . gobblers.
        rs = "^\\.*" + rs + "\\.*$";
        sum += workingCombos(parts, std::regex(rs), 0);
    }

    file.close();

    std::cout << "sum of combinations = " << sum << std::endl;
}

long workingCombos(std::string s, std::regex r, size_t modAt)
{
    size_t nextRep = s.substr(modAt).find_first_of("?");
    if (nextRep == std::string::npos)
        // apply it on this string.
        return std::regex_match(s, r);
    else
    {
        long combos = 0L;
        s[nextRep + modAt] = '.';
        combos += workingCombos(s, r, nextRep + modAt + 1);
        s[nextRep + modAt] = '#';
        combos += workingCombos(s, r, nextRep + modAt + 1);
        return combos;
    }
}
