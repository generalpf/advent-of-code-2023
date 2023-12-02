#include <iostream>
#include <fstream>
#include <list>
#include <stdlib.h>
using namespace std;

void scanGame(string game, int *red, int *green, int *blue);

int main()
{
    ifstream file;
    file.open("../data/day02.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    string line;
    long sum = 0;
    while (getline(file, line))
    {
        // heck a regex, let's do this manually.
        size_t colon = line.find_first_of(":");
        int gameNumber = atoi(line.substr(5, colon - 5).c_str());

        line = line.substr(colon + 2);

        int mostRed = 0;
        int mostGreen = 0;
        int mostBlue = 0;

        while (1)
        {
            size_t semicolon = line.find_first_of(";");
            string scanThis;
            if (semicolon == string::npos)
                scanThis = line;
            else
                scanThis = line.substr(0, semicolon);

            int red = 0, green = 0, blue = 0;
            scanGame(scanThis, &red, &green, &blue);

            if (red > mostRed) mostRed = red;
            if (green > mostGreen) mostGreen = green;
            if (blue > mostBlue) mostBlue = blue;

            if (semicolon == string::npos) break;
            line = line.substr(semicolon + 2);
        }

        if (mostRed <= 12 && mostGreen <= 13 && mostBlue <= 14)
            sum += gameNumber;
    }
    file.close();

    cout << "sum is " << sum << endl;
}

void scanGame(string game, int *red, int *green, int *blue)
{
    while (1)
    {
        size_t comma = game.find_first_of(",");
        string cubes;
        if (comma == string::npos)
            cubes = game;
        else
            cubes = game.substr(0, comma);

        size_t space = cubes.find_first_of(" ");
        int number = atoi(cubes.substr(0, space).c_str());
        string color = cubes.substr(space + 1);
        if (color == "red")
            *red = number;
        else if (color == "green")
            *green = number;
        else if (color == "blue")
            *blue = number;

        if (comma == string::npos) break;
        game = game.substr(comma + 2);
    }
}
