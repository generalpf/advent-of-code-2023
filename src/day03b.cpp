#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool is_symbol(char c);
int partNumberContaining(string line, size_t offset);
int partNumberStarting(string line, size_t offset);
int partNumberEnding(string line, size_t offset);

int main()
{
    ifstream file;
    file.open("../data/day03.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    const string digits = "0123456789";

    long sum = 0;
    // go row by row.
    for (size_t i = 0; i < lines.size(); i++) {
        // find the next gear in the string.
        size_t offset = 0;
        while (1)
        {
            size_t nextGear = lines[i].find_first_of("*", offset);
            if (nextGear == string::npos) break;    // no more gears in this row;

            vector<int> adjacentPartNumbers;
            // look for a part number to the left.
            if (nextGear > 0 && isdigit(lines[i][nextGear - 1]))
            {
                int partNumber = partNumberEnding(lines[i], nextGear - 1);
                adjacentPartNumbers.push_back(partNumber);
            }
            // look for a part number to the right.
            if (nextGear < lines[i].length() - 1 && isdigit(lines[i][nextGear + 1]))
            {
                int partNumber = partNumberStarting(lines[i], nextGear + 1);
                adjacentPartNumbers.push_back(partNumber);
            }
            if (i > 0)
            {
                // look for a part number above.
                if (isdigit(lines[i - 1][nextGear]))
                {
                    int partNumber = partNumberContaining(lines[i - 1], nextGear);
                    adjacentPartNumbers.push_back(partNumber);
                }
                else
                {
                    // look for a part number above and to the left.
                    if (nextGear > 0 && isdigit(lines[i - 1][nextGear - 1]))
                    {
                        int partNumber = partNumberEnding(lines[i - 1], nextGear - 1);
                        adjacentPartNumbers.push_back(partNumber);
                    }
                    // look for a part number above and to the right.
                    if (nextGear < lines[i].length() - 1 && isdigit(lines[i - 1][nextGear + 1]))
                    {
                        int partNumber = partNumberStarting(lines[i - 1], nextGear + 1);
                        adjacentPartNumbers.push_back(partNumber);
                    }
                }
            }
            if (i < lines.size() - 1)
            {
                // look for a part number below.
                if (isdigit(lines[i + 1][nextGear]))
                {
                    int partNumber = partNumberContaining(lines[i + 1], nextGear);
                    adjacentPartNumbers.push_back(partNumber);
                }
                else
                {
                    // look for a part number below and to the left.
                    if (nextGear > 0 && isdigit(lines[i + 1][nextGear - 1]))
                    {
                        int partNumber = partNumberEnding(lines[i + 1], nextGear - 1);
                        adjacentPartNumbers.push_back(partNumber);
                    }
                    // look for a part number below and to the right.
                    if (nextGear < lines[i].length() - 1 && isdigit(lines[i + 1][nextGear + 1]))
                    {
                        int partNumber = partNumberStarting(lines[i + 1], nextGear + 1);
                        adjacentPartNumbers.push_back(partNumber);
                    }
                }
            }

            if (adjacentPartNumbers.size() == 2)
                sum += adjacentPartNumbers[0] * adjacentPartNumbers[1];

            offset = nextGear + 1;
        }
    }

    cout << "sum is " << sum << endl;
}

bool is_symbol(char c)
{
    return !(isdigit(c) || c == '.');
}

int partNumberContaining(string line, size_t offset)
{
    size_t j = offset;
    string partNumber;
    while (1)
    {
        partNumber = line[j] + partNumber;
        if (j == 0 || !isdigit(line[j - 1])) break;
        --j;
    }
    if (offset < line.length() - 1)
    {
        j = offset + 1;
        while (1)
        {
            partNumber = partNumber + line[j];
            if (j == 0 || !isdigit(line[j + 1])) break;
            ++j;
        }
    }
    return stoi(partNumber);
}

int partNumberStarting(string line, size_t offset)
{
    size_t j = offset;
    string partNumber;
    while (1)
    {
        partNumber += line[j];
        if (j == line.length() - 1 || !isdigit(line[j + 1])) break;
        ++j;
    }
    return stoi(partNumber);
}

int partNumberEnding(string line, size_t offset)
{
    size_t j = offset;
    string partNumber;
    while (1)
    {
        partNumber = line[j] + partNumber;
        if (j == 0 || !isdigit(line[j - 1])) break;
        --j;
    }
    return stoi(partNumber);
}
