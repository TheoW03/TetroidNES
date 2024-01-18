#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Memory.h"
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
        for (size_t i = 0; i < line.size(); i++)
        {
            instructions.push_back((uint8_t)line[i]);
        }
    }
    write_16bit(0xFFFC, 0x8000);
    return instructions;
}
void modify_for_NESfile(vector<uint8_t> &instructions)
{
    write_16bit(0xFFFC, 0x8600);
    for (size_t i = 0; i < 4; i++)
        instructions.pop_back();
}