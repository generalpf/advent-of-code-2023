#include <iostream>
#include <fstream>
#include <vector>

struct Pos
{
    size_t x = -1, y = -1;

    bool undefined() { return x == -1 && y == -1; }
    friend bool operator==(const Pos& a, const Pos& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Pos& a, const Pos& b) { return !(a == b); }
};

enum Direction
{
    none, up, down, horizontal
};

Pos nextPipe(Pos from, Pos thatIsNot, std::vector<std::string>& grid);
Direction directionBetween(Pos from, Pos to);

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
    std::vector<std::vector<Direction>> directions;
    std::string line;
    while (getline(file, line))
    {
        grid.push_back(line);
        directions.push_back(std::vector<Direction>(line.length()));
    }

    file.close();

    Pos start;
    for (size_t row = 0; row < grid.size() && start.undefined(); row++)
        for (size_t column = 0; column < grid[0].size() && start.undefined(); column++)
            if (grid[row][column] == 'S')
                start = Pos { .x = column, .y = row };

    // find any adjacent pipe that's connected to this.
    Pos pos = nextPipe(start, Pos(), grid);
    Pos afterStart = pos;   // it's a surprise tool that will help us later!
    Pos prev = start;
    while (pos != start)
    {
        Pos next = nextPipe(pos, prev, grid);
        directions[pos.y][pos.x] = directionBetween(prev, next);
        prev = pos;
        pos = next;
    }
    // update start position, a special case.
    directions[start.y][start.x] = directionBetween(prev, afterStart);

    long inLoop = 0L;

    // row-by-row, detect empty or non-path squares that are between and odd number of pipes.
    for (size_t y = 0; y < grid.size(); y++)
    {
        int upBalance;
        Direction lastDir;
        for (size_t x = 0; x < grid[y].size(); x++)
        {
            if (directions[y][x] == up)
            {
                if (lastDir != up) 
                {
                    ++upBalance;
                    lastDir = up;
                }
            }
            else if (directions[y][x] == down)
            {
                if (lastDir != down)
                {
                    --upBalance;
                    lastDir = down;
                }
            }
            else if (directions[y][x] == horizontal)
                continue;
            else if (upBalance != 0)
                ++inLoop;
        }
    }

    std::cout << "squares in loop = " << inLoop << std::endl;
}

Pos nextPipe(Pos from, Pos thatIsNot, std::vector<std::string>& grid)
{
    char at = grid[from.y][from.x];
    // look right.
    if ((at == 'S' || at == '-' || at == 'F' || at == 'L') && from.x < grid[from.y].length() - 1)
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

Direction directionBetween(Pos from, Pos to)
{
    if (from.y < to.y)
        return down;
    else if (from.y > to.y)
        return up;
    else
        return horizontal;
}