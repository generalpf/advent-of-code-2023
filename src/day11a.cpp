#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Pos
{
    size_t x, y;

    friend bool operator==(const Pos& a, const Pos& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Pos& a, const Pos& b) { return !(a == b); }
};

int main()
{
    std::ifstream file;
    file.open("../data/day11.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (getline(file, line))
    {
        grid.push_back(line);
    }

    file.close();

    std::vector<size_t> columnsWithoutGalaxies, rowsWithoutGalaxies;

    for (size_t x = 0; x < grid.front().length(); x++)
    {
        bool foundGalaxy = false;
        for (size_t y = 0; y < grid.size(); y++)
            if (grid[y][x] == '#')
            {
                foundGalaxy = true;
                break;
            }
        if (!foundGalaxy)
            columnsWithoutGalaxies.push_back(x);
    }
    std::vector<Pos> galaxies;
    for (size_t y = 0; y < grid.size(); y++)
    {
        bool foundGalaxy = false;
        // no jumping out early, we are tagging all the galaxies as we go.
        for (size_t x = 0; x < grid[y].length(); x++)
            if (grid[y][x] == '#')
            {
                foundGalaxy = true;
                galaxies.push_back(Pos { .x = x, .y = y });
            }
        if (!foundGalaxy)
            rowsWithoutGalaxies.push_back(y);
    }

    long sum = 0L;
    for (size_t i = 0; i < galaxies.size() - 1; i++)
        for (size_t j = i + 1; j < galaxies.size(); j++)
        {
            size_t fromX = std::min(galaxies[i].x, galaxies[j].x);
            size_t toX = std::max(galaxies[i].x, galaxies[j].x);
            size_t fromY = std::min(galaxies[i].y, galaxies[j].y);
            size_t toY = std::max(galaxies[i].y, galaxies[j].y);

            size_t distance = (toX - fromX) + (toY - fromY);

            // add 1 for each gap row or column that would have expanded.
            auto rowsCrossed = std::count_if(rowsWithoutGalaxies.begin(), rowsWithoutGalaxies.end(), [fromY, toY](size_t row) { return row > fromY && row < toY; });
            auto colsCrossed = std::count_if(columnsWithoutGalaxies.begin(), columnsWithoutGalaxies.end(), [fromX, toX](size_t col) { return col > fromX && col < toX; });

            sum += (distance + rowsCrossed + colsCrossed);
        }

    std::cout << "sum of distances = " << sum << std::endl;
}
