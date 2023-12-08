#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define ALL_CARDS "23456789TJQKA"

enum Rank
{
    highCard, onePair, twoPair, threeOfAKind, fullHouse, fourOfAKind, fiveOfAKind
};

class Hand
{
public:
    Hand(std::string s)
    {
        cards = s.substr(0, 5);
        rank = parseRank(cards);
        bid = stol(s.substr(6));
    }
    Hand(const Hand& hand)
    {
        cards = hand.cards;
        rank = hand.rank;
        bid = hand.bid;
    }

    std::string getCards() { return cards; }
    Rank getRank() { return rank; }
    long getBid() { return bid; }

    friend bool operator<(const Hand& a, const Hand& b)
    {
        if (a.rank == b.rank)
        {
            for (int i = 0; i < 5; i++)
            {
                size_t aIndex = std::string(ALL_CARDS).find_first_of(a.cards[i]);
                size_t bIndex = std::string(ALL_CARDS).find_first_of(b.cards[i]);
                if (aIndex != bIndex) return aIndex < bIndex;
            }
            return false;
        }
        return a.rank < b.rank;
    }

private:
    std::string cards;
    Rank rank;
    long bid;

    Rank parseRank(std::string s)
    {
        for (auto& c : std::string(ALL_CARDS))
        {
            auto count = std::count(s.begin(), s.end(), c);
            if (count == 5) return fiveOfAKind;
            if (count == 4) return fourOfAKind;
            if (count == 3)
            {
                for (auto& d : std::string(ALL_CARDS))
                {
                    if (c == d) continue;
                    auto count = std::count(s.begin(), s.end(), d);
                    if (count == 2) return fullHouse;
                }
                return threeOfAKind;
            }
            if (count == 2)
            {
                for (auto& d : std::string(ALL_CARDS))
                {
                    if (c == d) continue;
                    auto count = std::count(s.begin(), s.end(), d);
                    if (count == 3) return fullHouse;
                    if (count == 2) return twoPair;
                }
                return onePair;
            }
        }
        return highCard;
    }
};

int main()
{
    std::ifstream file;
    file.open("../data/day07.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::vector<Hand> hands;
    std::string line;
    while (getline(file, line))
    {
        Hand hand(line);
        hands.push_back(hand);
    }

    file.close();

    std::sort(hands.begin(), hands.end());

    long sum = 0L;
    for (int i = hands.size() - 1; i >= 0; i--)
        sum += (i + 1) * hands[i].getBid();

    std::cout << "sum = " << sum << std::endl;
}
