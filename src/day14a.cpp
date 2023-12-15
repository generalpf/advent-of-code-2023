#include <iostream>
#include <fstream>
#include <vector>

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
    {
        grid.push_back(line);
    }
    file.close();

    for (size_t i = 0; i < grid.size() - 1; i++)
        for (size_t y = 0; y < grid.size() - 1; y++)
            for (size_t x = 0; x < grid[y].length(); x++)
                if (grid[y][x] == '.' && grid[y + 1][x] == 'O')
                {
                    grid[y][x] = 'O';
                    grid[y + 1][x] = '.';
                }

    long sum = 0;

    for (size_t y = 0; y < grid.size(); y++)
        for (size_t x = 0; x < grid[y].length(); x++)
            if (grid[y][x] == 'O')
                sum += (grid.size() - y);

    std::cout << "sum is " << sum << std::endl;
}
