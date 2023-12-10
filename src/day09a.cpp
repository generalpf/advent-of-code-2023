#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<long> longsInString(std::string s);
std::vector<long> derivative(std::vector<long> values);
bool allZeroes(std::vector<long> values);
long nextValue(std::vector<long> values);

int main()
{
    std::ifstream file;
    file.open("../data/day09.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::string line;
    long sum = 0L;
    while (getline(file, line))
    {
        std::vector<long> values = longsInString(line);
        long next = nextValue(values);
        sum += next;
    }

    file.close();

    std::cout << "sum = " << sum << std::endl;
}

std::vector<long> longsInString(std::string s)
{
    int offset = 0;
    std::vector<long> v;
    while (true)
    {
        size_t space = s.substr(offset).find_first_of(" ");
        std::string convertThis = s.substr(offset, space);
        v.push_back(stol(convertThis));
        if (space == std::string::npos) break;
        offset += space + 1;
    }
    return v;
}

std::vector<long> derivative(std::vector<long> values)
{
    std::vector<long> result;
    for (int i = 0; i < values.size() - 1; i++)
        result.push_back(values[i + 1] - values[i]);
    return result;
}

bool allZeroes(std::vector<long> values)
{
    return std::all_of(values.cbegin(), values.cend(), [](long l) { return l == 0; });
}

long nextValue(std::vector<long> values)
{
    std::vector<std::vector<long>> derivatives;
    derivatives.push_back(values);
    while (!allZeroes(derivatives.back()))
        derivatives.push_back(derivative(derivatives.back()));
    long next = 0L;
    for (int i = derivatives.size()  - 2; i >= 0; i--)
        next = derivatives[i].back() + next;
    return next;
}