#include <iostream>
#include "PPU.h"
#include "LoadRom.h"
using namespace std;

#ifndef BUS_H
#define BUS_H
class Bus
{
private:
    uint8_t memory[0x800];
    uint16_t reset_vector;
    Rom rom;
    PPU ppu;
    // ppu
public:
    Bus();
    Bus(Rom rom);
    uint8_t read_8bit(uint16_t address);
    void write_8bit(uint16_t address, uint8_t value);
    uint16_t read_16bit(uint16_t address);
    void write_16bit(uint16_t address, uint16_t value);
};
#endif