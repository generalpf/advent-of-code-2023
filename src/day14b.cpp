#include <iostream>
#include <fstream>
#include <vector>

void rollNorth(std::vector<std::string>& grid);
void rollWest(std::vector<std::string>& grid);
void rollSouth(std::vector<std::string>& grid);
void rollEast(std::vector<std::string>& grid);
long load(std::vector<std::string>& grid);

int main()
{
    std::ifstream file;
    file.open("../data/day14.txt");
    if (!file.is_open())
    {
        std::cerr << "could not open data file." << std::endl;
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(file, line))
        grid.push_back(line);
    file.close();

    const long n = 1000;
    std::vector<std::string> gridAfterNCycles;
    const long totalCycles = 1000000000L;
    long i = 0;
    while (i < totalCycles)
    {
        rollNorth(grid);
        rollWest(grid);
        rollSouth(grid);
        rollEast(grid);
        
        if (i == n - 1)
            gridAfterNCycles = grid;
        else if (grid == gridAfterNCycles)
        {
            long repeatLength = i - (n - 1);
            std::cout << "repeat length is " << repeatLength << std::endl;
            long cyclesLeft = totalCycles - i;
            long repeatsLeft = cyclesLeft / repeatLength;
            i += repeatsLeft * repeatLength;
            std::cout << "advancing to " << i << std::endl;
        }
        ++i;
    }

    std::cout << "load = " << load(grid) << std::endl;
}

void rollNorth(std::vector<std::string>& grid)
{
    for (size_t i = 0; i < grid.size() - 1; i++)
        for (size_t y = 0; y < grid.size() - 1; y++)
            for (size_t x = 0; x < grid[y].length(); x++)
                if (grid[y][x] == '.' && grid[y + 1][x] == 'O')
                {
                    grid[y][x] = 'O';
                    grid[y + 1][x] = '.';
                }
}

void rollWest(std::vector<std::string>& grid)
{
    for (size_t i = 0; i < grid.front().length() - 1; i++)
        for (size_t x = 0; x < grid.front().length() - 1; x++)
            for (size_t y = 0; y < grid.size(); y++)
                if (grid[y][x] == '.' && grid[y][x + 1] == 'O')
                {
                    grid[y][x] = 'O';
                    grid[y][x + 1] = '.';
                }
}

void rollSouth(std::vector<std::string>& grid)
{
    for (size_t i = 0; i < grid.size() - 1; i++)
        for (size_t y = grid.size() - 1; y > 0; y--)
            for (size_t x = 0; x < grid[y].length(); x++)
                if (grid[y][x] == '.' && grid[y - 1][x] == 'O')
                {
                    grid[y][x] = 'O';
                    grid[y - 1][x] = '.';
                }
}

void rollEast(std::vector<std::string>& grid)
{
    for (size_t i = 0; i < grid.front().length() - 1; i++)
        for (size_t x = grid.front().length() - 1; x > 0; x--)
            for (size_t y = 0; y < grid.size(); y++)
                if (grid[y][x] == '.' && grid[y][x - 1] == 'O')
                {
                    grid[y][x] = 'O';
                    grid[y][x - 1] = '.';
                }
}

long load(std::vector<std::string>& grid)
{
    long sum = 0;

    for (size_t y = 0; y < grid.size(); y++)
        for (size_t x = 0; x < grid[y].length(); x++)
            if (grid[y][x] == 'O')
                sum += (grid.size() - y);

    return sum;
}
