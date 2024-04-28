#include "Bus.h"
#include "PPU.h"
#include "APU.h"
#include <SFML/Graphics.hpp>

#define TOP_STACK 0x1ff
#define BOTTOM_STACK 0x100
#define STACK_RESET 0xfd

// const static uint8_t ram_end = 0x0FFF;
// const static uint8_t ppu_end = 0x2000;

Bus::Bus() {}

Bus::Bus(Rom rom)
{
    this->clock_cycles = 0;
    this->stored_instructions[0] = 0;
    this->stored_instructions[1] = 0;
    this->program_counter = 0;

    this->rom = rom;
    PPU ppu(rom.CHR, rom.mirror);
    this->ppu = ppu; // test
    APU APU();
    this->apu = apu; // test
    this->stack_pointer = STACK_RESET;
    this->stack = BOTTOM_STACK + stack_pointer;
}
uint16_t Bus::get_PC()
{
    return this->program_counter;
}

/**
 * Returns the most recently stored instruction.
 *
 * @return Most recently stored instruction.
 */
uint8_t Bus::get_current_instruction()
{
    return stored_instructions[1];
}

/**
 * Increments the program counter, updates the instructions stored in the pipeline and returns the previous current instruction.
 *
 * @return Previous current instruction.
 */
uint8_t Bus::fetch_next()
{

    uint8_t current_instruction = get_current_instruction();
    stored_instructions[1] = stored_instructions[0];
    program_counter++;
    stored_instructions[0] = rom.PRG[this->program_counter - reset_vector];
    return current_instruction;
}

void Bus::fill(uint16_t pc)
{
    stored_instructions[0] = rom.PRG[(pc + 1) - reset_vector];
    stored_instructions[1] = rom.PRG[(pc - reset_vector)];
    clock_cycles += 2;
    this->program_counter = pc;
    program_counter++;
}

uint8_t Bus::read_8bit(uint16_t address)
{
    this->clock_cycles++;
    if (address < 0x1FFF)
    {
        uint16_t mirror_address = address & 0x7ff;
        return v_memory[mirror_address];
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        uint16_t mirror_address = address & 0x2007;
    }
    else if (address >= 0x8000 && address <= 0xFFFB)
    {
        return fetch_next();
    }
    return 0;
}

void Bus::write_8bit(uint16_t address, uint8_t value)
{
    this->clock_cycles++;

    if (address <= 0x1FFF)
    {
        uint16_t mirror_address = address & 0x7ff;
        v_memory[mirror_address] = value;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        uint16_t mirror_address = address & 0x2007;
    }
    else if (address >= 0x8000 && address <= 0xFFFB)
    {
        cout << "Segmentation Fault (Core Dumped)" << endl;
        exit(EXIT_FAILURE);
    }

    // memory[address] = value;
}

uint16_t Bus::read_16bit(uint16_t address)
{
    clock_cycles += 2;

    if (address < 0x1FFF)
    {
        uint16_t mirror_address = address & 0x7ff;
        uint16_t value = (uint16_t)(v_memory[mirror_address + 1] << 8) | v_memory[mirror_address];
        return value;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        uint16_t mirror_address = address & 0x2007;
    }
    else if (address == 0xFFFC)
    {
        return reset_vector;
    }
    else if (address >= 0x8000 && address <= 0xFFFB)
    {

        uint8_t lsb = fetch_next();

        uint8_t msb = fetch_next();
        return (uint16_t)(msb << 8) | lsb;
    }
    return 0;
}

void Bus::write_16bit(uint16_t address, uint16_t value)
{
    clock_cycles += 2;

    if (address < 0x1FFF)
    {
        uint16_t mirror_address = address & 0x7ff;
        uint8_t msb = (uint8_t)(value >> 8);
        uint8_t lsb = (uint8_t)(value & 0xFF);
        v_memory[mirror_address] = lsb;
        v_memory[mirror_address + 1] = msb;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        uint16_t mirror_address = address & 0x2007;
    }
    else if (address == 0xFFFC)
    {
        reset_vector = value;
    }
    else if (address >= 0x8000 && address <= 0xFFFB)
    {
        cout << "Segmentation Fault (Core Dumped)" << endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Prints the clock cycle count to cout and sets that count to zero.
 */
void Bus::print_clock()
{
    cout << "Clock: " << clock_cycles << endl;
    this->clock_cycles = 0;
}
void Bus::push_stack8(uint8_t value)
{
    this->stack_pointer--;
    this->stack = (stack_pointer + BOTTOM_STACK);
    write_8bit((stack_pointer + BOTTOM_STACK), value);
}
uint8_t Bus::pop_stack8()
{
    uint8_t value = read_8bit(this->stack_pointer + BOTTOM_STACK);
    this->stack_pointer++;
    this->stack = (stack_pointer + BOTTOM_STACK);
    return value;
}
void Bus::push_stack16(uint16_t value)
{
    push_stack8(value >> 8);
    push_stack8(value & 0xff);
}
uint16_t Bus::pop_stack16()
{
    uint8_t lsb = pop_stack8();
    uint8_t msb = pop_stack8();
    return (uint16_t)(msb << 8) | lsb;
}
uint8_t Bus::get_stack_pointer()
{
    return this->stack_pointer;
}

void Bus::set_stack_pointer(uint8_t value)
{
    this->stack_pointer = value;
    this->stack = (stack_pointer + BOTTOM_STACK);
}

void Bus::print_stack()
{
    printf("stack: %x \n", this->stack);
    printf("stack_pointer %x \n", this->stack_pointer);
}
