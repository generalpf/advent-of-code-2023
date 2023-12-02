#include <iostream>
#include <fstream>
#include <list>
using namespace std;

int main()
{
    ifstream file;
    file.open("../data/day01.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    string line;
    long sum = 0;
    string strings[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    while (getline(file, line))
    {
        list<int> digits;
        for (int i = 0; i < line.length(); i++)
        {
            if (isdigit(line[i]))
            {
                digits.push_back(line[i] - '0');
                continue;
            }
            for (int j = 1; j < 10; j++)
            {
                if (line.substr(i).find(strings[j]) == 0)
                {
                    digits.push_back(j);
                    continue;
                }
            }
        }
        int firstDigit = digits.front();
        int lastDigit = digits.back();
        cout << line << " " << firstDigit << " " << lastDigit << endl;
        sum += 10 * firstDigit + lastDigit;
    }
    file.close();

    cout << "sum is " << sum << endl;
}
