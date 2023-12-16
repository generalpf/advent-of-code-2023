#include <iostream>
#include <fstream>
#include <vector>

enum Direction
{
    north, east, south, west
};

struct Laser
{
    // can't use size_t bcause the laser starts at (-1, 0).
    long x, y;
    Direction d;
    bool dead;
};

bool recordVisit(Laser* l, std::vector<std::vector<int>>& visitDirections);

int main()
{
    std::ifstream file;
    file.open("../data/day16.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (getline(file, line))
        grid.push_back(line);

    file.close();

    std::vector<Laser> lasers;
    lasers.push_back(Laser { .x = -1L, .y = 0L, .d = east });

    std::vector<std::vector<int>> visitDirections;
    for (size_t y = 0; y < grid.size(); y++)
    {
        std::vector<int> row;
        for (size_t x = 0; x < grid[y].length(); x++)
            row.push_back(0);
        visitDirections.push_back(row);
    }

    while (true)
    {
       // don't run new lasers.
        size_t nLasers = lasers.size();
        bool anythingNew = false;
        for (size_t i = 0; i < nLasers; i++)
        {
            Laser* lIter = &lasers[i];
            if (lIter->dead) continue;
            switch (lIter->d)
            {
                case north:
                    --(lIter->y);
                    break;
                case east:
                    ++(lIter->x);
                    break;
                case south:
                    ++(lIter->y);
                    break;
                case west:
                    --(lIter->x);
                    break;
            }
            if (lIter->y < 0 || lIter->y >= grid.size() || lIter->x < 0 || lIter->x >= grid.front().length())
            {
                lIter->dead = true;
                continue;
            }

            if (recordVisit(lIter, visitDirections)) anythingNew = true;
            
            if (grid[lIter->y][lIter->x] == '/')
            {
                switch (lIter->d)
                {
                    case north:
                        lIter->d = east;
                        break;
                    case east:
                        lIter->d = north;
                        break;
                    case south:
                        lIter->d = west;
                        break;
                    case west:
                        lIter->d = south;
                        break;
                }
            }
            else if (grid[lIter->y][lIter->x] == '\\')
            {
                switch (lIter->d)
                {
                    case north:
                        lIter->d = west;
                        break;
                    case east:
                        lIter->d = south;
                        break;
                    case south:
                        lIter->d = east;
                        break;
                    case west:
                        lIter->d = north;
                }
            }
            else if (grid[lIter->y][lIter->x] == '|')
            {
                switch (lIter->d)
                {
                    case north:
                    case south:
                        break;
                    case east:
                    case west:
                        lIter->d = north;
                        if (recordVisit(lIter, visitDirections)) anythingNew = true;
                        // make a new laser going south.
                        Laser l { .x = lIter->x, .y = lIter->y, .d = south };
                        if (recordVisit(&l, visitDirections)) anythingNew = true;
                        lasers.push_back(l);
                }
            }
            else if (grid[lIter->y][lIter->x] == '-')
            {
                switch (lIter->d)
                {
                    case east:
                    case west:
                        break;
                    case north:
                    case south:
                        lIter->d = west;
                        if (recordVisit(lIter, visitDirections)) anythingNew = true;
                        // make a new laser going east.
                        Laser l { .x = lIter->x, .y = lIter->y, .d = east };
                        if (recordVisit(&l, visitDirections)) anythingNew = true;
                        lasers.push_back(l);
                }
            }
        }
        if (!anythingNew) break;
    }

    long visits = 0L;
    for (size_t y = 0; y < visitDirections.size(); y++)
        for (size_t x = 0; x < visitDirections[y].size(); x++)
            if (visitDirections[y][x] != 0)
                ++visits;
    std::cout << "visits = " << visits << std::endl;
}

bool recordVisit(Laser* l, std::vector<std::vector<int>>& visitDirections)
{
    int bits = (int) pow(2.0, l->d);
    bool result = !(visitDirections[l->y][l->x] & bits);
    visitDirections[l->y][l->x] |= bits;
    return result;
}