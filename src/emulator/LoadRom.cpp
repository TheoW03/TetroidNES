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
    byte_t ident[4]; // should be the words "NES 0x1a"
    byte_t prg_size; // size of PRG
    byte_t chr_size; // size of chr
    union
    {
        struct
        {
            unsigned mapper_lower : 4; // lower bits of mapper
            unsigned four_screen : 1;  // if 4 screen
            unsigned trainer : 1;      // trainer. Ie the PRG is a 512 offset
            unsigned battery : 1;      // for zelda (it used a battery to save )
            unsigned vertical : 1;     // if vertical
        };
        byte_t val;
    } flag6;

    union
    {
        struct
        {
            unsigned mapper_upper : 4; // upper bits of mapper
            unsigned inesverif : 4;    // 0xc to verify if its ines 1.0 format
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
    memcpy(&nes_header, instructions.data(), sizeof(NESHeader));

    // uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    if (                                 //
        (nes_header.ident[0] != 'N'      //
         && nes_header.ident[1] != 'E'   //
         && nes_header.ident[2] != 'S'   //
         && nes_header.ident[3] != 0x1a) // all man style should be the default in the VS code formatiro
        || nes_header.flag7.inesverif == 0xc)
    {
        std::cout << "not NES Rom or NES 1.0 format" << std::endl;
        exit(EXIT_FAILURE);
    }
    // uint8_t map = (instructions[7] & 0b11110000) | (instructions[6] >> 4);
    rom.mapper = nes_header.flag7.mapper_upper | nes_header.flag6.mapper_lower;
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
    size_t chr_start = prg_start + prg_rom;
    size_t chr_size = prg_start + prg_rom + chr_rom;
    for (size_t i = chr_start; i < chr_size; i++)
    {
        rom.CHR.push_back(instructions[i]);
    }
    return rom;
}