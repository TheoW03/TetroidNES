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
#include <filesystem>
#include "../../src/emulator/InstructionMap.h"
namespace fs = std::filesystem;

#define PC_RESET 0x8000
#define PC_END 0xffff
// using instructionPointer = void (*)(AddressMode, CPU &);

uint8_t current_instruction = 0;
uint8_t param = 0;

CPU run(CPU cpu, std::string file_name);

CPU init(std::string file_name)
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
	CPU cpu;

	// std::cout <<bus.read_16bit(0xfffc) <<
	bus.fill(bus.read_16bit(0xfffc));
	cpu.A_Reg = 0;
	cpu.status = 0;
	cpu.X_Reg = 0;
	cpu.Y_Reg = 0;
	cpu.bus = bus;
	cpu.bus.clock_cycles = 0;
	std::string window_name = fs::path(file_name).filename().replace_extension().string();
	return run(cpu, window_name);
}
void printCPU_stats(CPU cpu)
{
	printf("Accumaltor: decimal: %d hexa: 0x%x \n", cpu.A_Reg, cpu.A_Reg);
	printf("X Register: decimal: %d hexa: 0x%x \n", cpu.X_Reg, cpu.X_Reg);
	printf("Y Register:decimal: %d hexa: 0x%x \n", cpu.Y_Reg, cpu.Y_Reg);
	printf("Program Counter: 0x%X \n", cpu.bus.get_PC());
	// printf("Stack Pointer: 0x%X \n", cpu.stack_pointer);
	cpu.bus.print_stack();
	std::bitset<7> status(cpu.status);
	cpu.bus.print_clock();
	std::cout << "cpu status register: 0b" << status << std::endl;
}

void HandleNMIInterrupts(CPU &cpu)
{
	cpu.bus.push_stack8(cpu.status);

	cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
	cpu.bus.fetch_next();
	cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
	set_interrupt_disabled(1, cpu);
}
// void HandleIRQInterrupts(CPU &cpu)
// {
// 	set_brk(cpu, 1);
// 	cpu.bus.push_stack8(cpu.status);
// 	cpu.bus.fetch_next();
// 	cpu.bus.push_stack16(cpu.bus.get_PC() - 1);

// 	cpu.bus.fill(cpu.bus.read_16bit(0xfffe));
// }
/**
 * Executes actual code
 */
CPU run(CPU cpu, std::string window_name)
{

	sf::RenderWindow window(sf::VideoMode(800, 600), window_name);

	window.setFramerateLimit(144);
	sf::Texture texture;
	texture.create(200, 200);
	float scaleX = window.getSize().x / (float)(texture.getSize().x);
	float scaleY = window.getSize().y / (float)(texture.getSize().y);
	sf::Sprite sprite(texture);
	sprite.setScale(scaleX, scaleY);

	while (cpu.bus.get_PC() < PC_END && window.isOpen())
	{
		bool brk = false;

		// printf("%x \n", current_instruction);
// if(window.)
#pragma region SFML boiler plat
		for (auto event = sf::Event{}; window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				program_success(cpu);
				cpu.error_code = EXIT_SUCCESS;
				return cpu;
			}
		}

		cpu.bus.write_8bit(0xfe, ((uint8_t)rand() % 16 + 1));

#pragma endregion
		cpu.bus.tick();
		if (cpu.bus.NMI_interrupt())
		{
			HandleNMIInterrupts(cpu);
		}
		current_instruction = cpu.bus.fetch_next();

		cpu.bus.render(texture, 0, 0);
		window.clear(); // Change this to the desired color
		window.draw(sprite);
		window.display();
		if (InstructionValid(current_instruction))
		{
			Instruction a = GetInstruction(current_instruction);
			a.i(a.addressmode, cpu);
		}
		else
		{
			// printf("Current instruction 0x%x", current_instruction, instructionMap[cpu.bus.get_PC()]);
			program_failure("Unrecognized instruction encountered", cpu, 1);
			cpu.error_code = EXIT_FAILURE;
			return cpu;
		}
	}
	return cpu;
}
