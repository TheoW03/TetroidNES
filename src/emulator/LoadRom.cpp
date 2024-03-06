#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Memory.h"
#include <cstdio>
using namespace std;

struct Rom
{
    vector<uint8_t> PRG;
    vector<uint8_t> CHR;
    uint8_t mapper;
};
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
Rom modify_for_NESfile(vector<uint8_t> &instructions)
{
    Rom rom;
    uint16_t map_info;

    write_16bit(0xFFFC, 0x8600);
    for (size_t i = 0; i < 4; i++)
        instructions.pop_back();
    uint8_t prg_rom = instructions[0];
    // instructions.pop_back();
    uint8_t chr_rom = instructions[1];
    // instructions.pop_back();
    uint8_t control_byte1 = instructions[2];
    // instructions.pop_back();
    uint8_t control_byte2 = instructions[3];
    // instructions.pop_back();
    uint8_t size_ofprgRam = instructions[4];
    // instructions.pop_back();
    // for (size_t i = 0; i < 7; i++)
    // instructions.pop_back();
    // write_16bit(0xFFFC, 0x8600);
    // rom.instructions = instructions;
    return rom;
}