#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

bool is_symbol(char c);

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
        // find the next number in the string.
        size_t offset = 0;
        while (1)
        {
            size_t nextDigit = lines[i].find_first_of(digits, offset);
            if (nextDigit == string::npos) break;   // no more parts in this row.

            // where does this part number end?
            size_t nextNonDigit = lines[i].find_first_not_of(digits, nextDigit);
            size_t lastDigit = nextNonDigit == string::npos ? lines[i].length() - 1 : nextNonDigit - 1;

            // pull out the part number.
            int partNumber = stoi(lines[i].substr(nextDigit, lastDigit - offset + 1));

            // look for a symbol around it.
            bool foundSymbol = false;
            // anything to the left?
            if (nextDigit != 0) foundSymbol = is_symbol(lines[i][nextDigit - 1]);
            // anything to the right?
            if (!foundSymbol && nextNonDigit != string::npos) foundSymbol = is_symbol(lines[i][nextNonDigit]);
            // figure out start and stop indices (inclusive) for lines above and below.
            size_t start = nextDigit == 0 ? 0 : nextDigit - 1;
            size_t stop = nextNonDigit == string::npos ? lines[i].length() - 1 : nextNonDigit;
            // look above
            if (!foundSymbol && i > 0)
            {
                for (size_t j = start; j <= stop; j++)
                {
                    if (is_symbol(lines[i - 1][j]))
                    {
                        foundSymbol = true;
                        break;
                    }
                }
            }
            // look below.
            if (!foundSymbol && i < lines.size() - 1)
            {
                for (size_t j = start; j <= stop; j++)
                {
                    if (is_symbol(lines[i + 1][j]))
                    {
                        foundSymbol = true;
                        break;
                    }
                }
            }
            if (foundSymbol)
                sum += partNumber;

            // move the offset forward, unless this ended the string.
            if (nextNonDigit == string::npos) break;
            offset = nextNonDigit;
        }
    }

    cout << "sum is " << sum << endl;
}

bool is_symbol(char c)
{
    return !(isdigit(c) || c == '.');
}
