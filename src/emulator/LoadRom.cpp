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
    // cout << "input: " << endl;
    while (getline(infile, line))
    {
        uint8_t num = (line[1] << 8) | line[0];
        instructions.push_back(num);
    }

    return instructions;
}