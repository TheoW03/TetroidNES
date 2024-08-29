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

// easier to understand over doing bytes and bit operations
typedef uint8_t byte_t;
struct NESHeader
{
    byte_t ident[4];
    byte_t prg_size;
    byte_t chr_size;
    union
    {
        struct
        {
            unsigned mapper_lower : 4;
            unsigned four_screen : 1;
            unsigned trainer : 1;
            unsigned battery : 1;
            unsigned vertical : 1;
        };
        byte_t val;
    } flag6;

    union
    {
        struct
        {
            unsigned mapper_upper : 4;
            unsigned inesverif : 2;
            unsigned inesverif2 : 2;
        };
        byte_t val;
    } flag7;
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
    NESHeader nes_header;
    std::memcpy(&nes_header, instructions.data(), sizeof(NESHeader));

    // uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    if (instructions[0] != 'N' && instructions[1] != 'E' && instructions[2] != 'S' && instructions[3] != 0x1a 
    (instructions[7] & 0b1101) == 0xc)
    {
        std::cout << "not NES Rom or NES 1.0 format" << std::endl;
        exit(EXIT_FAILURE);
    }
    // uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    uint8_t map = nes_header.flag7.mapper_upper | nes_header.flag6.mapper_lower;
    rom.mapper = map;

    size_t prg_rom = nes_header.prg_size * PRG_ROM_SIZE;
    // if (prg_rom ==)
    size_t chr_rom = nes_header.chr_size * CHR_ROM_SIZE;
    // uint8_t control_byte1 = instructions[6];
    bool four_screen = nes_header.flag6.four_screen;
    bool vertical_mirroring = nes_header.flag6.vertical;
    // bool four_screen = (control_byte1 & 0b1000) != 0;
    // bool vertical_mirroring = (control_byte1 & 0b1) != 0;
    rom.mirror = (four_screen)                          ? MirrorType::FOUR_SCREEN
                 : (!four_screen && vertical_mirroring) ? MirrorType::VERTICAL
                                                        : MirrorType::HORIZONTAL;
    uint16_t prg_start = 16 + (512 * nes_header.flag6.trainer);
    // printf("%d \n", prg_start + prg_rom)
    for (size_t i = prg_start; i < prg_rom + prg_start; i++)
    {
        rom.PRG.push_back(instructions[i]);
    }
    for (size_t i = prg_start + prg_rom; i < prg_start + prg_rom + chr_rom; i++)
    {
        rom.CHR.push_back(instructions[i]);
    }
    return rom;
}