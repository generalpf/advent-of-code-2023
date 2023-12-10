#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

int main()
{
    ifstream file;
    file.open("../data/day06.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    string timesString;
    getline(file, timesString);
    timesString = timesString.substr(9);
    string distancesString;
    getline(file, distancesString);
    distancesString = distancesString.substr(9);

    file.close();

    timesString.erase(remove_if(timesString.begin(), timesString.end(), ::isspace), timesString.end());
    long long time = stoll(timesString);
    distancesString.erase(remove_if(distancesString.begin(), distancesString.end(), ::isspace), distancesString.end());
    long long distanceToBeat = stoll(distancesString);

    long waysToWin = 0;
    for (long long j = 1; j < time; j++)
    {
        long long distance = j * (time - j);
        if (distance > distanceToBeat) ++waysToWin;
    }

    cout << "ways to win = " << waysToWin << endl;
}
