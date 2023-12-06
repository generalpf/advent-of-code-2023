#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <climits>
using namespace std;

struct Mapper
{
    long to, from, range;

    long map(long l)
    {
        return (l >= from && l < from + range) ? (to + l - from) : -1;
    }
};

vector<long> longsInString(string s);
vector<Mapper> mappersAfterLinePrefix(string prefix, ifstream& file);
long throughMappers(long l, vector<Mapper> v);

int main()
{
    ifstream file;
    file.open("../data/day05.txt");
    if (!file.is_open())
    {
        cerr << "could not open data file." << endl;
        return 1;
    }

    string line;
    getline(file, line);
    size_t colon = line.find_first_of(":");
    vector<long> seeds = longsInString(line.substr(colon + 2));

    vector<vector<Mapper>> pipeline;
    pipeline.push_back(mappersAfterLinePrefix("seed-to-soil map:", file));
    pipeline.push_back(mappersAfterLinePrefix("soil-to-fertilizer map:", file));
    pipeline.push_back(mappersAfterLinePrefix("fertilizer-to-water map:", file));
    pipeline.push_back(mappersAfterLinePrefix("water-to-light map:", file));
    pipeline.push_back(mappersAfterLinePrefix("light-to-temperature map:", file));
    pipeline.push_back(mappersAfterLinePrefix("temperature-to-humidity map:", file));
    pipeline.push_back(mappersAfterLinePrefix("humidity-to-location map:", file));

    file.close();

    long lowestLocation = LONG_MAX;
    for (auto& seed : seeds)
    {
        long l = seed;
        for (auto& pipe : pipeline)
            l = throughMappers(l, pipe);
        if (l < lowestLocation) lowestLocation = l;
    }

    cout << "lowest location = " << lowestLocation << endl;
}

vector<long> longsInString(string s)
{
    int offset = 0;
    vector<long> v;
    while (true)
    {
        size_t space = s.substr(offset).find_first_of(" ");
        string convertThis = s.substr(offset, space);
        v.push_back(stol(convertThis));
        if (space == string::npos) break;
        offset += space + 1;
    }
    return v;
}

vector<Mapper> mappersAfterLinePrefix(string prefix, ifstream& file)
{
    string line;

    // eat until we find the prefix line
    while (getline(file, line))
        if (line == prefix) break;

    vector<Mapper> mappers;
    while (getline(file, line))
    {
        if (line.length() == 0) break;

        vector<long> stuff = longsInString(line);
        // if we die, we die. not every man truly lives.
        Mapper m { .to = stuff[0], .from = stuff[1], .range = stuff[2] };
        mappers.push_back(m);
    }

    return mappers;
}

long throughMappers(long l, vector<Mapper> v)
{
    for (vector<Mapper>::iterator iter = v.begin(); iter != v.end(); iter++)
    {
        long to = iter->map(l);
        if (to != -1) return to;
    }
    return l;
}