#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
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

    vector<int> times;
    vector<int> bestDistances;
    regex r("(\\d+)(?:\\s+|$)+");
    auto numbersBegin = sregex_iterator(timesString.begin(), timesString.end(), r);
    for (sregex_iterator i = numbersBegin; i != sregex_iterator(); i++)
        times.push_back(stoi((*i).str()));
    numbersBegin = sregex_iterator(distancesString.begin(), distancesString.end(), r);
    for (sregex_iterator i = numbersBegin; i != sregex_iterator(); i++)
        bestDistances.push_back(stoi((*i).str()));

    long product = 1L;
    for (int i = 0; i < times.size(); i++)
    {
        int time = times[i];
        int distanceToBeat = bestDistances[i];
        int waysToWin = 0;
        for (int j = 1; j < time; j++)
        {
            int distance = j * (time - j);
            if (distance > distanceToBeat) ++waysToWin;
        }
        product *= waysToWin;
    }

    cout << "product = " << product << endl;
}
