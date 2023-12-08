#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define ALL_CARDS "J23456789TQKA"

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
        auto jokerCount = std::count(s.begin(), s.end(), 'J');
        if (jokerCount >= 4) return fiveOfAKind;
        for (auto& c : std::string(ALL_CARDS))
        {
            if (c == 'J') continue;

            auto count = std::count(s.begin(), s.end(), c);
            if (count + jokerCount == 5) return fiveOfAKind;
            if (count + jokerCount == 4) return fourOfAKind;
            if (count == 3)
            {
                for (auto& d : std::string(ALL_CARDS))
                {
                    if (c == d || d == 'J') continue;
                    auto count = std::count(s.begin(), s.end(), d);
                    if (count + jokerCount == 2) return fullHouse;
                }
                return threeOfAKind;
            }
            if (count == 2)
            {
                // 55JJJ would have triggered as five-of-a-kind above.
                // 556JJ would have triggered as four-of-a-kind above.
                // 5567J is a three-of-a-kind and is better than two pair.
                for (auto& d : std::string(ALL_CARDS))
                {
                    if (c == d || d == 'J') continue;
                    auto count = std::count(s.begin(), s.end(), d);
                    if (count == 3) return fullHouse;
                    // consider 5566J, that's a full house.
                    // but 55667 is two pair.
                    if (count == 2) return jokerCount == 1 ? fullHouse : twoPair;
                }
                // we got here, only found a pair, never found another pair,
                // and there are 0 or 1 jokers.
                return jokerCount == 1 ? threeOfAKind : onePair;
            }
        }
        // we didn't find a card with two or more copies.
        // so, other than jokers, we're all distinct cards.
        // if there were three or four jokers, we'd have caught it above.
        // 567JJ
        if (jokerCount == 2) return threeOfAKind;
        // 5678J
        if (jokerCount == 1) return onePair;
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
    for (size_t i = 0; i < hands.size(); i++)
        sum += (i + 1) * hands[i].getBid();

    std::cout << "sum = " << sum << std::endl;
}
