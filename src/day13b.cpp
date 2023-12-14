#include <iostream>
#include <fstream>
#include <vector>

long score(std::vector<std::string>& grid);
bool horizontalReflection(std::vector<std::string>& grid, size_t justBeforeIndex, bool allowSmudge);
bool rowsAreTheSame(std::vector<std::string>& grid, size_t row1, size_t row2, bool* usedSmudge);
bool verticalReflection(std::vector<std::string>& grid, size_t justBeforeIndex, bool allowSmudge);
bool colsAreTheSame(std::vector<std::string>& grid, size_t col1, size_t col2, bool* usedSmudge);

int main()
{
    std::ifstream file;
    file.open("../data/day13.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    long total = 0L;
    std::vector<std::string> grid;
    std::string line;
    while (getline(file, line))
    {
        if (line.length() == 0)
        {
            long s = score(grid);
            std::cout << "grid width " << grid.front().length() << " height " << grid.size() << " score " << s << std::endl;
            total += s;
            grid.erase(grid.begin(), grid.end());
        }
        else
            grid.push_back(line);
    }
    long s = score(grid);
    std::cout << "grid width " << grid.front().length() << " height " << grid.size() << " score " << s << std::endl;
    total += s;

    file.close();

    std::cout << "total score = " << total << std::endl;
}

long score(std::vector<std::string>& grid)
{
    /*
     looking for horizontal reflection.
     height 8 -> indices 0 - 7 -> should start comparing rows 3, 4; 2, 5; 1, 6; 0, 7;
     height 9 -> indices 0 - 8 -> should
       start comparing rows 3, 4; 2, 5; 1, 6; 0, 7; 8 is extra
       start comparing rows 4, 5; 3, 6; 2, 7; 1, 8; 0 is extra
    */
    size_t xWithoutSmudge = -1, yWithoutSmudge = -1;
    for (size_t y = 1; y < grid.size(); y++)
        if (horizontalReflection(grid, y, false))
        {
            yWithoutSmudge = y;
            break;
        }
    for (size_t x = 1; x < grid.front().length(); x++)
        if (verticalReflection(grid, x, false))
        {
            xWithoutSmudge = x;
            break;
        }

    if (xWithoutSmudge == -1 && yWithoutSmudge == -1) return 0L;

    for (size_t y = 1; y < grid.size(); y++)
        if (y != yWithoutSmudge && horizontalReflection(grid, y, true))
            return 100 * y;

    for (size_t x = 1; x < grid.front().length(); x++)
        if (x != xWithoutSmudge && verticalReflection(grid, x, true))
            return x;

    return 0L;
}

bool horizontalReflection(std::vector<std::string>& grid, size_t justBeforeIndex, bool allowSmudge)
{
    size_t goingUp = justBeforeIndex - 1;
    size_t goingDown = justBeforeIndex;
    bool usedSmudge = !allowSmudge;
    while (true)
    {
        if (!rowsAreTheSame(grid, goingUp, goingDown, &usedSmudge)) return false;
        // size_t is unsigned and will wrap.
        if (goingUp == 0 || goingDown == grid.size() - 1) return true;
        --goingUp;
        ++goingDown;
    }
    return true;
}

bool rowsAreTheSame(std::vector<std::string>& grid, size_t row1, size_t row2, bool* usedSmudge)
{
    size_t hammingDistance = 0;
    for (int i = 0; i < grid[row1].length(); i++)
        if (grid[row1][i] != grid[row2][i])
            if (++hammingDistance == 2) return false;
    if (hammingDistance == 0) return true;
    if (hammingDistance == 1 && !*usedSmudge)
    {
        *usedSmudge = true;
        return true;
    }
    return false;
}

bool verticalReflection(std::vector<std::string>& grid, size_t justBeforeIndex, bool allowSmudge)
{
    size_t goingLeft = justBeforeIndex - 1;
    size_t goingRight = justBeforeIndex;
    bool usedSmudge = !allowSmudge;
    while (true)
    {
        if (!colsAreTheSame(grid, goingLeft, goingRight, &usedSmudge)) return false;
        // size_t is unsigned and will wrap.
        if (goingLeft == 0 || goingRight == grid.front().length() - 1) return true;
        --goingLeft;
        ++goingRight;
    }
    return true;
}

bool colsAreTheSame(std::vector<std::string>& grid, size_t col1, size_t col2, bool* usedSmudge)
{
    size_t hammingDistance = 0;
    for (int i = 0; i < grid.size(); i++)
        if (grid[i][col1] != grid[i][col2])
            if (++hammingDistance == 2) return false;
    if (hammingDistance == 0) return true;
    if (hammingDistance == 1 && !*usedSmudge)
    {
        *usedSmudge = true;
        return true;
    }
    return false;
}

