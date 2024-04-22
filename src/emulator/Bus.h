#include <iostream>
#include "PPU.h"
#include "LoadRom.h"
#include "APU.h"
using namespace std;

#ifndef BUS_H
#define BUS_H
class Bus
{
private:
    uint8_t v_memory[0x800];
    uint16_t reset_vector;
    Rom rom;
    PPU ppu;
    APU apu;
    uint16_t program_counter;
    uint8_t stack_pointer;
    uint16_t stack;

public:
    int clock_cycles;
    uint8_t stored_instructions[2];

    Bus();
    Bus(Rom rom);

    uint8_t get_current_instruction();
    uint8_t fetch_next();
    void fill(uint16_t program_counter);

    uint8_t read_8bit(uint16_t address);
    void write_8bit(uint16_t address, uint8_t value);
    uint16_t read_16bit(uint16_t address);
    void write_16bit(uint16_t address, uint16_t value);
    void print_clock();
    uint16_t get_PC();

    // stack methods. provides a more clean way of stack reads and writes
    void push_stack8(uint8_t value);
    uint8_t pop_stack8();

    void push_stack16(uint16_t value);
    uint16_t pop_stack16();

    uint8_t get_stack_pointer();

    void set_stack_pointer(uint8_t value);
    void print_stack(); // prints true value of stack
};
#endif