#include <iostream>
#include <fstream>
#include <vector>

long score(std::vector<std::string>& grid);
bool horizontalReflection(std::vector<std::string>& grid, size_t justBeforeIndex);
bool verticalReflection(std::vector<std::string>& grid, size_t justBeforeIndex);

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
    for (size_t y = 1; y < grid.size(); y++)
        if (horizontalReflection(grid, y))
            return 100 * y;
    for (size_t x = 1; x < grid.front().length(); x++)
        if (verticalReflection(grid, x))
            return x;
    return 0L;
}

bool horizontalReflection(std::vector<std::string>& grid, size_t justBeforeIndex)
{
    size_t goingUp = justBeforeIndex - 1;
    size_t goingDown = justBeforeIndex;
    while (true)
    {
        if (grid[goingDown] != grid[goingUp]) return false;
        // size_t is unsigned and will wrap.
        if (goingUp == 0 || goingDown == grid.size() - 1) return true;
        --goingUp;
        ++goingDown;
    }
    return true;
}

bool verticalReflection(std::vector<std::string>& grid, size_t justBeforeIndex)
{
    size_t goingLeft = justBeforeIndex - 1;
    size_t goingRight = justBeforeIndex;
    while (true)
    {
        for (size_t y = 0; y < grid.size(); y++)
            if (grid[y][goingLeft] != grid[y][goingRight]) return false;
        // size_t is unsigned and will wrap.
        if (goingLeft == 0 || goingRight == grid.front().length() - 1) return true;
        --goingLeft;
        ++goingRight;
    }
    return true;
}
