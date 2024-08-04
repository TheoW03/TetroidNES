#include <bitset>
#include <cstdio>
#include <iostream>
#include <map>
#include <thread>
#include <SFML/Graphics.hpp>

// #include <pthread.h>
#include "NESError.h"
// #include <unistd.h>

#include "BitOperations.h"

#include "AddressMode.h"
#include "Bus.h"
#include "Instructions.h"
#include "LoadRom.h"
#include "StatusRegister.h"

#define PC_RESET 0x8000
#define PC_END 0xffff
using instructionPointer = void (*)(AddressMode, CPU &);

uint8_t current_instruction = 0;
uint8_t param = 0;

struct Instruction
{
	instructionPointer i;
	AddressMode addressmode;
};
std::map<uint8_t, Instruction> instructionMap;

void run(CPU cpu, std::string file_name);
void initializeInstructionMap();

/**
 * Initializes the CPU object and memories.
 */
void init(std::string file_name)
{
	initializeInstructionMap();
	// vector<uint8_t> test = {0xa9, 0xa, 0xa0, 0xa, 0xa2, 0xa, 0x00};
	// std::vector<uint8_t> test = {0xad, 0x07, 0x20, 0xa9, 0xa};
	// Rom rom;
	// rom.PRG = test;
	// rom.CHR = test;
	// rom.mapper = 0;
	// rom.mirror = MirrorType::FOUR_SCREEN;
	// Bus bus(rom, PC_RESET);
	std::vector<uint8_t> v = file_tobyte_vector(file_name);
	Bus bus(load_rom(v), 0x8000);
	// printf("%x \n", )
	CPU cpu;
	bus.fill(PC_RESET);
	cpu.A_Reg = 0;
	cpu.status = 0;
	cpu.X_Reg = 0;
	cpu.Y_Reg = 0;
	cpu.bus = bus;
	cpu.bus.clock_cycles = 0;

	run(cpu, file_name);
}

/**
 * Loads instructions and their associated function in the emulator into a map.
 * May be read for documenting which codes are associated with each instruction.
 */
void initializeInstructionMap()
{

	using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this

#pragma region LDA
	instructionMap.insert(make_pair(0xA9, Instruction{(instructionPointer)LDA, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xAD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0xB9, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0xA1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0xB1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region LDX

	instructionMap.insert(make_pair(0xA2, Instruction{(instructionPointer)LDX, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE_Y}));
	instructionMap.insert(make_pair(0xAE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE_Y}));

#pragma endregion

#pragma region LDY

	instructionMap.insert(make_pair(0xA0, Instruction{(instructionPointer)LDY, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xAC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE_X}));

#pragma endregion

#pragma region ADC
	instructionMap.insert(make_pair(0x69, Instruction{(instructionPointer)ADC, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0x65, Instruction{(instructionPointer)ADC, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x75, Instruction{(instructionPointer)ADC, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x6D, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x7D, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0x79, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0x71, Instruction{(instructionPointer)ADC, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0x61, Instruction{(instructionPointer)ADC, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region SBC
	instructionMap.insert(make_pair(0xE9, Instruction{(instructionPointer)SBC, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xE5, Instruction{(instructionPointer)SBC, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xF5, Instruction{(instructionPointer)SBC, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xED, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xFD, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0xF9, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0xF1, Instruction{(instructionPointer)SBC, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0xE1, Instruction{(instructionPointer)SBC, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region AND
	instructionMap.insert(make_pair(0x29, Instruction{(instructionPointer)AND, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0x25, Instruction{(instructionPointer)AND, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x35, Instruction{(instructionPointer)AND, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x2D, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x3D, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0x39, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0x21, Instruction{(instructionPointer)AND, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0x31, Instruction{(instructionPointer)AND, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region ORA
	instructionMap.insert(make_pair(0x09, Instruction{(instructionPointer)ORA, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0x05, Instruction{(instructionPointer)ORA, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x15, Instruction{(instructionPointer)ORA, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x0D, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x1D, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0x19, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0x01, Instruction{(instructionPointer)ORA, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0x11, Instruction{(instructionPointer)ORA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region ROL/ROR
	instructionMap.insert(make_pair(0x26, Instruction{(instructionPointer)ROL, AddressMode::ACCUMULATOR}));
	instructionMap.insert(make_pair(0x2A, Instruction{(instructionPointer)ROL, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x36, Instruction{(instructionPointer)ROL, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x2E, Instruction{(instructionPointer)ROL, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x3E, Instruction{(instructionPointer)ROL, AddressMode::ABSOLUTE_X}));

	instructionMap.insert(make_pair(0x6A, Instruction{(instructionPointer)ROR, AddressMode::ACCUMULATOR}));
	instructionMap.insert(make_pair(0x66, Instruction{(instructionPointer)ROR, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x76, Instruction{(instructionPointer)ROR, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x6E, Instruction{(instructionPointer)ROR, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x7E, Instruction{(instructionPointer)ROR, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region STX
	instructionMap.insert(make_pair(0x86, Instruction{(instructionPointer)STX, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x96, Instruction{(instructionPointer)STX, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x8E, Instruction{(instructionPointer)STX, AddressMode::ABSOLUTE}));

	instructionMap.insert(make_pair(0x84, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x94, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x8C, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE_Y}));

	instructionMap.insert(make_pair(0x85, Instruction{(instructionPointer)STA, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x95, Instruction{(instructionPointer)STA, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x8D, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x9D, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0x99, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0x81, Instruction{(instructionPointer)STA, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0x91, Instruction{(instructionPointer)STA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region bitshift
	instructionMap.insert(make_pair(0x4A, Instruction{(instructionPointer)LSR, AddressMode::ACCUMULATOR}));
	instructionMap.insert(make_pair(0x46, Instruction{(instructionPointer)LSR, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x56, Instruction{(instructionPointer)LSR, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x4E, Instruction{(instructionPointer)LSR, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x5E, Instruction{(instructionPointer)LSR, AddressMode::ABSOLUTE_X}));

	instructionMap.insert(make_pair(0x0A, Instruction{(instructionPointer)ASL, AddressMode::ACCUMULATOR}));
	instructionMap.insert(make_pair(0x06, Instruction{(instructionPointer)ASL, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x16, Instruction{(instructionPointer)ASL, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x0E, Instruction{(instructionPointer)ASL, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x1E, Instruction{(instructionPointer)ASL, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region XOR
	instructionMap.insert(make_pair(0x49, Instruction{(instructionPointer)EOR, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0x45, Instruction{(instructionPointer)EOR, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x55, Instruction{(instructionPointer)EOR, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0x4D, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x5D, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0x59, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0x41, Instruction{(instructionPointer)EOR, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0x51, Instruction{(instructionPointer)EOR, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region DEC
	instructionMap.insert(make_pair(0xCA, Instruction{(instructionPointer)DEX, AddressMode::IMPLIED}));
	instructionMap.insert(make_pair(0x88, Instruction{(instructionPointer)DEY, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xC6, Instruction{(instructionPointer)DEC, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xD6, Instruction{(instructionPointer)DEC, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xCE, Instruction{(instructionPointer)DEC, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xDE, Instruction{(instructionPointer)DEC, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region INC
	instructionMap.insert(make_pair(0xE8, Instruction{(instructionPointer)INX, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xC8, Instruction{(instructionPointer)INY, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xE6, Instruction{(instructionPointer)INC, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xF6, Instruction{(instructionPointer)INC, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xEE, Instruction{(instructionPointer)INC, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xFE, Instruction{(instructionPointer)INC, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region Clear/set Flags
	instructionMap.insert(make_pair(0x18, Instruction{(instructionPointer)CLC, AddressMode::IMPLIED})); //"implied"

	instructionMap.insert(make_pair(0x38, Instruction{(instructionPointer)SEC, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xD8, Instruction{(instructionPointer)CLD, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xF8, Instruction{(instructionPointer)SED, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x78, Instruction{(instructionPointer)SEI, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x58, Instruction{(instructionPointer)CLI, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xB8, Instruction{(instructionPointer)CLV, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x40, Instruction{(instructionPointer)RTI, AddressMode::IMPLIED}));
#pragma endregion

#pragma region JMP
	instructionMap.insert(make_pair(0x4C, Instruction{(instructionPointer)JMP, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0x6C, Instruction{(instructionPointer)JMP, AddressMode::INDIRECT}));
#pragma endregion

#pragma region Compare
	instructionMap.insert(make_pair(0xC9, Instruction{(instructionPointer)CMP, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xC5, Instruction{(instructionPointer)CMP, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xD5, Instruction{(instructionPointer)CMP, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xCD, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xDD, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0xD9, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0xC1, Instruction{(instructionPointer)CMP, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0xD1, Instruction{(instructionPointer)CMP, AddressMode::INDIRECT_Y}));

	instructionMap.insert(make_pair(0xC0, Instruction{(instructionPointer)CPY, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xC4, Instruction{(instructionPointer)CPY, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xCC, Instruction{(instructionPointer)CPY, AddressMode::ABSOLUTE}));

	instructionMap.insert(make_pair(0xE0, Instruction{(instructionPointer)CPX, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xE4, Instruction{(instructionPointer)CPX, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xEC, Instruction{(instructionPointer)CPX, AddressMode::ABSOLUTE}));

	instructionMap.insert(make_pair(0x24, Instruction{(instructionPointer)BIT, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0x2C, Instruction{(instructionPointer)BIT, AddressMode::ABSOLUTE}));

#pragma endregion

#pragma region Conditional Branching
	instructionMap.insert(make_pair(0xF0, Instruction{(instructionPointer)BEQ, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0xD0, Instruction{(instructionPointer)BNE, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0x90, Instruction{(instructionPointer)BCC, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0xB0, Instruction{(instructionPointer)BCS, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0x10, Instruction{(instructionPointer)BPL, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0x30, Instruction{(instructionPointer)BMI, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0x50, Instruction{(instructionPointer)BVC, AddressMode::RELATIVE}));

	instructionMap.insert(make_pair(0x70, Instruction{(instructionPointer)BVS, AddressMode::RELATIVE}));
#pragma endregion

#pragma region Subroutine
	instructionMap.insert(make_pair(0x20, Instruction{(instructionPointer)JSR, AddressMode::ABSOLUTE}));

	instructionMap.insert(make_pair(0x60, Instruction{(instructionPointer)RTS, AddressMode::IMPLIED}));
#pragma endregion

#pragma region Pull register from stack
	instructionMap.insert(make_pair(0x28, Instruction{(instructionPointer)PLP, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x08, Instruction{(instructionPointer)PHP, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x68, Instruction{(instructionPointer)PLA, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x48, Instruction{(instructionPointer)PHA, AddressMode::IMPLIED}));
#pragma endregion

#pragma region Transfer
	instructionMap.insert(make_pair(0xAA, Instruction{(instructionPointer)TAX, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x8A, Instruction{(instructionPointer)TXA, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x9A, Instruction{(instructionPointer)TXS, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xBA, Instruction{(instructionPointer)TSX, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0x98, Instruction{(instructionPointer)TYA, AddressMode::IMPLIED}));

	instructionMap.insert(make_pair(0xA8, Instruction{(instructionPointer)TAY, AddressMode::IMPLIED}));
#pragma endregion
}
void printCPU_stats(CPU cpu)
{
	printf("Accumaltor: %x \n", cpu.A_Reg);
	printf("X Register: %d \n", cpu.X_Reg);
	printf("Y Register: %d \n", cpu.Y_Reg);
	printf("Program Counter: 0x%X \n", cpu.bus.get_PC());
	// printf("Stack Pointer: 0x%X \n", cpu.stack_pointer);
	cpu.bus.print_stack();
	std::bitset<7> status(cpu.status);
	cpu.bus.print_clock();
	std::cout << "cpu status register: 0b" << status << std::endl;
}
/**
 * Executes instructions in a loop and handles proper/improper exits.
 */
void run(CPU cpu, std::string render_name)
{
	bool brk = false;
	sf::RenderWindow window(sf::VideoMode(800, 600), render_name);
	window.setFramerateLimit(144);
	sf::Texture texture;
	texture.create(200, 200);
	float scaleX = window.getSize().x / (float)(texture.getSize().x);
	float scaleY = window.getSize().y / (float)(texture.getSize().y);
	sf::Sprite sprite(texture);
	sprite.setScale(scaleX, scaleY);

	while (cpu.bus.get_PC() < PC_END && window.isOpen())
	{
// if(window.)
#pragma region SFML boiler plat
		for (auto event = sf::Event{}; window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				program_success(cpu);
				// printCPU_stats(cpu);
				// window.close();
				// std::cout << "" << std::endl;
				// std::cout << "\033[92mProgram has successfully ended" << std::endl;
				// std::cout << "exit code 0 \033[0m" << std::endl;
				// exit(EXIT_SUCCESS);
			}
		}

		cpu.bus.write_8bit(0xfe, ((uint8_t)rand() % 16 + 1));

#pragma endregion
		cpu.bus.tick();
		if (cpu.bus.NMI_interrupt())
		{
			cpu.bus.push_stack8(cpu.status);
			cpu.bus.fetch_next();
			cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
			cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
			set_interrupt_disabled(1, cpu);
		}
		current_instruction = cpu.bus.read_8bit(cpu.bus.get_PC());
		cpu.bus.render(texture, 0, 0);
		window.clear(); // Change this to the desired color
		window.draw(sprite);
		window.display();
		// Equivalent to, in English, if instructionMap contains current_instruction.
		if (instructionMap.find(current_instruction) != instructionMap.end()) // if instruction
		{

			Instruction a = instructionMap.at(current_instruction);
			a.i(a.addressmode, cpu);
		}
		else if (current_instruction == 0x00)
		{

			if (check_interrupt_disabled(cpu) != 0 || brk)
			{
				continue;
			}
			set_brk(cpu, 1);
			cpu.bus.push_stack8(cpu.status);
			cpu.bus.fetch_next();
			cpu.bus.push_stack16(cpu.bus.get_PC());
			printf("Halt instruction encountered.\n");
			printCPU_stats(cpu);
			cpu.bus.fill(PC_END);
			brk = true;
		}
		else if (current_instruction != 0xEA)
		{
			printf("Current instruction 0x%x", current_instruction, instructionMap[cpu.bus.get_PC()]);
			program_failure("Unrecognized instruction encountered", cpu, 1);
		}
	}
}
