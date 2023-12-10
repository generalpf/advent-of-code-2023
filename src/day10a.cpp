#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct Pos
{
    size_t x = -1, y = -1;

    bool undefined() { return x == -1 && y == -1; }
    friend bool operator==(const Pos& a, const Pos& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Pos& a, const Pos& b) { return !(a == b); }
};

Pos nextPipe(Pos from, Pos thatIsNot, std::vector<std::string>& grid);

int main()
{
    std::ifstream file;
    file.open("../data/day10.txt");
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

    Pos start;
    for (size_t row = 0; row < grid.size() && start.undefined(); row++)
        for (size_t column = 0; column < grid[0].size() && start.undefined(); column++)
            if (grid[row][column] == 'S')
                start = Pos { .x = column, .y = row };

    // find any adjacent pipe that's connected to this.
    Pos pos = nextPipe(start, Pos(), grid);
    Pos prev = start;
    int distance = 1;
    while (pos != start)
    {
        Pos next = nextPipe(pos, prev, grid);
        prev = pos;
        pos = next;
        ++distance;
    }

    std::cout << "max distance is " << ceil(distance / 2) << std::endl;
}

Pos nextPipe(Pos from, Pos thatIsNot, std::vector<std::string>& grid)
{
    char at = grid[from.y][from.x];
    // look right.
    if ((at == 'S' || at == '-' || at == 'F' || at == 'L') && from.x < grid.size() - 1)
    {
        Pos right { .x = from.x + 1, .y = from.y };
        if (right != thatIsNot)
        {
            char c = grid[from.y][from.x + 1];
            if (c == 'J' || c == '7' || c == '-' || c == 'S')
                return right;
        }
    }
    // look left.
    if ((at == 'S' || at == '-' || at == 'J' || at == '7') && from.x > 0)
    {
        Pos left { .x = from.x - 1, .y = from.y };
        if (left != thatIsNot)
        {
            char c = grid[from.y][from.x - 1];
            if (c == 'F' || c == 'L' || c == '-' || c == 'S')
                return left;
        }
    }
    // look below.
    if ((at == 'S' || at == '|' || at == 'F' || at == '7') && from.y < grid.size() - 1)
    {
        Pos below { .x = from.x, .y = from.y + 1 };
        if (below != thatIsNot)
        {
            char c = grid[from.y + 1][from.x];
            if (c == 'J' || c == 'L' || c == '|' || c == 'S')
                return below;
        }
    }
     // look above.
    if ((at == 'S' || at == '|' || at == 'J' || at == 'L') && from.y > 0)
    {
        Pos above { .x = from.x, .y = from.y - 1 };
        if (above != thatIsNot)
        {
            char c = grid[from.y - 1][from.x];
            if (c == '7' || c == 'F' || c == '|' || c == 'S')
                return above;
        }
    }
    return Pos { .x = (size_t) -1, .y = (size_t) -1 };
}
