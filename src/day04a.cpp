#include <iostream>
#include <fstream>
#include <set>
#include <iterator>
using namespace std;

int main()
{
    ifstream file;
    file.open("../data/day04.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    long sum = 0;

    string line;
    while (getline(file, line))
    {
        set<int> myNumbers, winningNumbers;
        size_t colonPos = line.find_first_of(":");
        size_t barPos = line.find_first_of("|");
        for (size_t i = colonPos + 1; i < barPos - 1; i += 3)
        {
            int number = stoi(line.substr(i, 3));
            myNumbers.insert(number);
        }
        for (size_t i = barPos + 1; i < line.length() - 2; i += 3)
        {
            int number = stoi(line.substr(i, 3));
            winningNumbers.insert(number);
        }
        // C++ stdlib sets don't do intersections?
        int points = 0;
        for (set<int>::const_iterator iter = myNumbers.cbegin(); iter != myNumbers.cend(); iter++)
            if (winningNumbers.find(*iter) != winningNumbers.cend())
                points = !points ? 1 : points * 2;
        sum += points;
    }
    file.close();

    cout << "sum is " << sum << endl;
}
