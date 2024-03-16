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
    ifstream infile(file_name, ios::binary); // Open the file for reading
    string line;
    if (!infile)
    {
        cerr << "Error opening file." << endl;
        exit(EXIT_FAILURE);
        // return 1;
    }
    while (infile)
    {
        char a = (uint8_t)infile.get();
        // char32_t b;
        // printf("%x \n", (uint8_t)a);
        instructions.push_back(a);

        // for (size_t i = 0; i < line.size(); i++)
        // {
        //     instructions.push_back((uint8_t)line[i]);
        //     printf("%x \n",line[i]);
        // }
    }
    // for (int i = 0; i < instructions.size(); i++)
    //     // printf("%x \n", instructions[i]);
    // while (getline(infile, line, 'n'))
    // {

    // for (size_t i = 0; i < line.size(); i++)
    // {
    //     instructions.push_back((uint8_t)line[i]);
    //     printf("%x \n",line[i]);
    // }
    // }
    write_16bit(0xFFFC, 0x8000);
    return instructions;
}
Rom modify_for_NESfile(vector<uint8_t> instructions)
{
    Rom rom;
    uint16_t map_info;

    write_16bit(0xFFFC, 0x8600);
    uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    if (instructions[0] != 'N' && instructions[1] != 'E' && instructions[2] != 'S' && instructions[3] != 0x1a)
    {
        cout << "not NES Rom" << endl;
        exit(EXIT_FAILURE);
    }
    if ((instructions[7] & 0b1101) == 0xc)
    {
        cout << "not NES 1.0 format" << endl;
        exit(EXIT_FAILURE);
    }

    uint8_t prg_rom = instructions[4];
    // instructions.pop_back();
    uint8_t chr_rom = instructions[5];
    // instructions.pop_back();
    uint8_t control_byte1 = instructions[6];
    // instructions.pop_back();
    uint8_t control_byte2 = instructions[7];
    // instructions.pop_back();
    uint8_t size_ofprgRam = instructions[8];
    // instructions.pop_back();
    // for (size_t i = 0; i < 7; i++)
    // instructions.pop_back();
    // write_16bit(0xFFFC, 0x8600);
    // rom.instructions = instructions;
    return rom;
}