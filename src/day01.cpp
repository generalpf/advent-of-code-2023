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
        // assuming index != string::npos because it's ok to crash here.
        size_t index = line.find_first_of("0123456789");
        firstDigit = line[index] - '0';
        index = line.find_last_of("0123456789");
        lastDigit = line[index] - '0';
        sum += 10 * firstDigit + lastDigit;
    }
    file.close();

    cout << "sum is " << sum << endl;
}
