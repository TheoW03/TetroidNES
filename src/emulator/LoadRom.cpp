#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>

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
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
};
std::vector<uint8_t> file_tobyte_vector(std::string file_name)
{
    std::vector<uint8_t> instructions;
    std::ifstream infile(file_name, std::ios::binary); // Open the file for reading
    std::string line;
    if (!infile)
    {
        std::cerr << "Error opening file." << std::endl;
        exit(EXIT_FAILURE);
    }
    while (infile)
    {
        uint8_t a = (uint8_t)infile.get();
        instructions.push_back(a);
    }
    return instructions;
}

Rom load_rom(std::vector<uint8_t> instructions)
{
    Rom rom;
    uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    rom.mapper = map;
    if (instructions[0] != 'N' && instructions[1] != 'E' && instructions[2] != 'S' && instructions[3] != 0x1a)
    {
        std::cout << "not NES Rom" << std::endl;
        exit(EXIT_FAILURE);
    }
    if ((instructions[7] & 0b1101) == 0xc)
    {
        std::cout << "NES 1.0 format" << std::endl;
        exit(EXIT_FAILURE);
    }

    size_t prg_rom = instructions[4] * PRG_ROM_SIZE;
    size_t chr_rom = instructions[5] * CHR_ROM_SIZE;
    uint8_t control_byte1 = instructions[6];
    bool four_screen = (control_byte1 & 0b1000) != 0;
    bool vertical_mirroring = (control_byte1 & 0b1) != 0;
    rom.mirror = (four_screen)                          ? MirrorType::FOUR_SCREEN
                 : (!four_screen && vertical_mirroring) ? MirrorType::VERTICAL
                                                        : MirrorType::HORIZONTAL;
    uint16_t prg_start = 16 + (512 * (((instructions[6] & 0b100)) != 0));
    printf("%d \n", prg_start);
    printf("%d \n", prg_rom);
    for (size_t i = prg_start; i <   prg_rom + prg_start - 1; i++)
    {
        rom.PRG.push_back(instructions[i]);
    }
    for (size_t i = prg_start + prg_rom; i < prg_start + prg_rom + chr_rom; i++)
    {
        rom.CHR.push_back(instructions[i]);
    }
    return rom;
}