#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Memory.h"
#include <cstdio>
using namespace std;

#define PRG_ROM_SIZE 16384
#define CHR_ROM_SIZE 8192
enum MirrorType
{
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN,
};
struct Rom
{
    vector<uint8_t> PRG;
    vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
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
    write_16bit(0xFFFC, 0x8600);
    uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    rom.mapper = map;
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

    uint16_t prg_rom = instructions[4] * PRG_ROM_SIZE;
    uint16_t chr_rom = instructions[5] * CHR_ROM_SIZE;
    uint8_t control_byte1 = instructions[6];
    bool four_screen = (control_byte1 & 0b1000) != 0;
    bool vertical_mirroring = (control_byte1 & 0b1) != 0;
    rom.mirror = (four_screen)                          ? MirrorType::FOUR_SCREEN
                 : (!four_screen && vertical_mirroring) ? MirrorType::VERTICAL
                                                        : MirrorType::HORIZONTAL;
    uint16_t prg_start = 16 + (512 * (((instructions[6] & 0b100)) != 0));
    for (size_t i = prg_start; i < prg_rom; i++)
    {
        rom.PRG.push_back(instructions[i]);
    }
    for (size_t i = prg_start + prg_rom; i < chr_rom; i++)
    {
        rom.CHR.push_back(instructions[i]);
    }
    uint8_t a = 0;

    return rom;
}