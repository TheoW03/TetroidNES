// #include "../include/Bus.h"
// #include "../include/PPU.h"
// #include "../include/APU.h"
#include <emulator/APU.h>
#include <emulator/Bus.h>
#include <emulator/PPU.h>
#include <SFML/Graphics.hpp>
#include <bitset>
#define TOP_STACK 0x1ff
#define BOTTOM_STACK 0x100
#define STACK_RESET 0xfd
// #include "test.h"
// const static uint8_t ram_end = 0x0FFF;
// const static uint8_t ppu_end = 0x2000;

Bus::Bus()
{
}

Bus::Bus(Rom rom, uint16_t pc_start)
{
    this->clock_cycles = 0;
    this->stored_instructions[0] = 0;
    this->stored_instructions[1] = 0;
    this->program_counter = 0;
    this->reset_vector = pc_start;
    this->rom = rom;
    PPU nes_ppu(rom.CHR, rom.mirror);
    joy_pad_byte1 = 0;
    this->ppu = nes_ppu; // test
    // this->ppu.chr_rom = rom.CHR;
    // this->ppu.mirrorType = rom.mirror;

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
    if (this->program_counter > 0xffff)
    {
        return 0;
    }

    uint8_t current_instruction = get_current_instruction();
    stored_instructions[1] = stored_instructions[0];
    // printf(" fetch: current_instrcution: 0x%x  pc: 0x%x \n", current_instruction, this->program_counter);
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
    // printf("current_instrcution: 0x%x  pc: 0x%x \n", current_instruction, this->program_counter);
    // printf(" fill: proram counter: 0x%x current: 0x%x \n", pc, stored_instructions[1]);

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
        // printf("ppuread: 0x%x \n", address);

        if (address >= 0x2008)
            return read_8bit(address & 0x2007);
        if (address == 0x2007)
            return this->ppu.read_PPU_data();
        else if (address == 0x2002)
            return this->ppu.read_status();
        else if (address == 0x2004)
        {
            return this->ppu.read_OAM_data();
        }
        else
        {
            this->stored_instructions[1] = 0x82;
            std::cout << "\033[91mforbidden access to PPU write only address\033[0m" << std::endl;
            printf("address 0x%x \n", address);
            std::cout << "" << std::endl;
            return -1;
        }
    }
    else if (address == 0x4014)
    {
        this->stored_instructions[1] = 0x82;
        std::cout << "\033[91mforbidden access to PPU write only address\033[0m" << std::endl;
        printf("address %x \n", address);
        std::cout << "" << std::endl;
        return -1;
    }
    else if (address == 0x4016)
    {
        return read_joypad();
    }
    else if (address == 0x4017)
    {
        return joy_pad_byte2;
    }
    else if (address >= 0x8000 && address <= 0xFFFF)
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
        // std::cout << "ppu write" << address << std::endl;

        if (address == 0x2000)
        {
            this->ppu.write_PPU_ctrl(value);
        }
        else if (address == 0x2001)
        {
            this->ppu.write_PPU_mask(value);
        }
        else if(address == 0x2003){
            this->ppu.write_OAM_data(value);
        }
        else if (address == 0x2006)
        {
            this->ppu.write_PPU_address(value);
        }
        else if (address == 0x2007)
        {
            this->ppu.write_PPU_data(value);
        }
        else if (address >= 0x2008)
            this->write_8bit(address & 0x2007, value);
        else
        {
            this->stored_instructions[1] = 0x82;
            std::cout << "\033[91mforbidden access to PPU write only address\033[0m" << std::endl;
            printf("address 0x%x \n", address);
            std::cout << "" << std::endl;
        }
    }
    else if (address == 0x4016)
    {
        strobe = (bool)value;
        button_idx = 0;
        // std::cout <<
        // joy_pad_byte1 = 0;
        // joy_pad_byte1 = value & 0b00000001;
        // return
    }
    // else if (address == 0x4017)
    // {
    //     joy_pad_byte2 = value;
    // }
    else if (address >= 0x8000 && address <= 0xFFFF)
    {
        this->stored_instructions[1] = 0x82;

        std::cout << "\033[91mAttempt to write into READ_ONLY_MEM\033[0m" << std::endl;
        printf("address 0x%x \n", address);
        std::cout << "" << std::endl;

        // exit(EXIT_FAILURE);
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
        return read_8bit(address + 1) << 8 | read_8bit(address);
    }
    else if (address >= 0x8000 && address <= 0xFFFF)
    {

        uint8_t lsb = rom.PRG[address - reset_vector];
        uint8_t msb = rom.PRG[(address + 1) - reset_vector];
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
        // uint16_t mirror_address = address & 0x2007;
        write_8bit(address + 1, value >> 8);
        write_8bit(address, value);
    }

    else if (address >= 0x8000 && address <= 0xFFFF)
    {
        this->stored_instructions[1] = 0x82;

        std::cout << "Attempt to write into READ_ONLY_MEM" << std::endl;
    }
}

/**
 * Prints the clock cycle count to cout and sets that count to zero.
 */
void Bus::print_clock()
{
    std::cout << "Clock: " << clock_cycles << std::endl;
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

void Bus::tick()
{
    this->ppu.tick(this->clock_cycles * 3);
}

void Bus::render(sf::Texture &texture, int bank, int tile)
{
    this->ppu.render(texture, bank, tile);
}

bool Bus::NMI_interrupt()
{
    return this->ppu.NMI_interrupt(this->clock_cycles * 3);
}

uint8_t Bus::read_joypad()
{
    if (button_idx > 7)
    {
        return 1;
    }
    uint8_t button = (joy_pad_byte1 << button_idx);
    if (strobe)
    {
        button_idx++;
    }
    return button;
}

void Bus::write_controller1(Controller value, int isPressed)
{
    // uint8_t v = (uint8_t) value;

    if (isPressed == 1)
        joy_pad_byte1 |= (uint8_t)value;
    else if (isPressed == 0)
        joy_pad_byte1 &= ~((uint8_t)(value));
}
