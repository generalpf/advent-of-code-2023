#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <iterator>
using namespace std;

struct Card
{
    int copies;
    int matches;
};

int main()
{
    ifstream file;
    file.open("../data/day04.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    vector<Card> cards;

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
        Card c;
        c.copies = 1;
        c.matches = 0;
        for (set<int>::const_iterator iter = myNumbers.cbegin(); iter != myNumbers.cend(); iter++)
            if (winningNumbers.find(*iter) != winningNumbers.cend())
                ++c.matches;
        cards.push_back(c);
    }
    file.close();

    int copies = 0;

    for (size_t i = 0; i < cards.size(); i++)
    {
        Card c = cards[i];
        for (size_t j = 1; j <= c.matches && (i + j) < cards.size(); j++)
        {
            cards[i + j].copies += c.copies;
            copies += c.copies;
        }
    }

    cout << "card count = " << cards.size() + copies << endl;
}
