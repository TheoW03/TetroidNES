#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
using namespace std;

vector<uint8_t> load_rom(string file_name)
{
    vector<uint8_t> instructions;
    vector<string> vec;
    ifstream infile(file_name); // Open the file for reading
    string line;
    while (getline(infile, line, 'n'))
    {
        for (int i = 0; i < line.size(); i++)
        {
            instructions.push_back((uint8_t)line[i]);
        }
    }

    return instructions;
}