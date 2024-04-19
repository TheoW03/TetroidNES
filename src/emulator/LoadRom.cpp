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
vector<uint8_t> file_tobyte_vector(string file_name)
{
    vector<uint8_t> instructions;
    vector<string> vec;
    ifstream infile(file_name, ios::binary); // Open the file for reading
    string line;
    if (!infile)
    {
        cerr << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }
    while (infile)
    {
        uint8_t a = (uint8_t)infile.get();
        instructions.push_back(a);
    }
    write_16bit(0xFFFC, 0x8000);
    return instructions;
}

Rom load_rom(vector<uint8_t> instructions)
{
    Rom rom;
    uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    rom.mapper = map;
    if (instructions[0] != 'N' && instructions[1] != 'E' && instructions[2] != 'S' && instructions[3] != 0x1a)
    {
        cout << "not NES Rom" << endl;
        exit(EXIT_FAILURE);
    }
    if ((instructions[7] & 0b1101) == 0xc)
    {
        cout << "NES 1.0 format" << endl;
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
    return rom;
}