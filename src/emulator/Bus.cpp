#include "Bus.h"
#include "PPU.h"
#include "APU.h"

using namespace std;

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
    PPU ppu(rom.CHR);
    this->ppu = ppu;
    APU APU();
    this->apu = apu;
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
    stored_instructions[0] = rom.PRG[this->program_counter - reset_vector];
    this->program_counter++;

    return current_instruction;
}

void Bus::fill(uint16_t pc)
{
    stored_instructions[1] = rom.PRG[pc - reset_vector];
    stored_instructions[0] = rom.PRG[(pc + 1) - reset_vector];
    this->program_counter = pc;
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
        return rom.PRG[address - reset_vector];
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
        return (uint16_t)(rom.PRG[(address - reset_vector) + 1] << 8) | rom.PRG[address - reset_vector];
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
