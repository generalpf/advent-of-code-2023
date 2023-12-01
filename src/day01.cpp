#include <iostream>
#include <fstream>
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
    while (getline(file, line))
    {
        char firstDigit;
        char lastDigit;
        for (int i = 0; i < line.length(); i++) {
            if (isnumber(line[i])){
                firstDigit = line[i] - '0';
                break;
            }
        }
        for (int i = line.length() - 1; i >= 0; i--) {
            if (isnumber(line[i])){
                lastDigit = line[i] - '0';
                break;
            }
        }
        sum += 10 * firstDigit + lastDigit;
    }
    file.close();

    cout << "sum is " << sum << endl;
}
