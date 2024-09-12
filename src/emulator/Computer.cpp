#include <bitset>
#include <cstdio>
#include <iostream>
#include <map>
#include <thread>
#include <SFML/Graphics.hpp>
#include "./NESError.h"
// #include <emulator/NESError.h>
#include <emulator/BitOperations.h>

#include <emulator/AddressMode.h>
// #include "Bus.h
#include <emulator/Instructions.h>
#include <emulator/LoadRom.h>
#include <emulator/StatusRegister.h>
#include <filesystem>
#include <emulator/InstructionMap.h>
namespace fs = std::filesystem;

#define PC_RESET 0x8000
#define PC_END 0xffff

uint8_t current_instruction = 0;

CPU run(CPU cpu, std::string file_name);

CPU init(std::string file_name)
{
	initializeInstructionMap();
	std::vector<uint8_t> v = file_tobyte_vector(file_name);
	Bus bus(load_rom(v), 0x8000);
	CPU cpu;
	bus.fill(bus.read_16bit(0xfffc));
	cpu.A_Reg = 0;
	cpu.status.val = 0;
	cpu.X_Reg = 0;
	cpu.Y_Reg = 0;
	cpu.bus = bus;
	cpu.bus.clock_cycles = 0;
	std::string window_name = fs::path(file_name)
								  .filename()
								  .replace_extension()
								  .string();
	return run(cpu, window_name);
}
void printCPU_stats(CPU cpu)
{
	printf("Accumaltor: decimal: %d hexa: 0x%x \n", cpu.A_Reg, cpu.A_Reg);
	printf("X Register: decimal: %d hexa: 0x%x \n", cpu.X_Reg, cpu.X_Reg);
	printf("Y Register:decimal: %d hexa: 0x%x \n", cpu.Y_Reg, cpu.Y_Reg);
	printf("Program Counter: 0x%X \n", cpu.bus.get_PC());
	cpu.bus.print_stack();
	std::bitset<7> status(cpu.status.val);
	cpu.bus.print_clock();
	std::cout << "cpu status register: 0b" << status << std::endl;
}

void HandleNMIInterrupts(CPU &cpu)
{
	cpu.bus.push_stack8(cpu.status.val);

	cpu.bus.push_stack16(cpu.bus.get_PC() - 1);
	cpu.bus.fetch_next();
	cpu.bus.fill(cpu.bus.read_16bit(0xfffa));
	set_interrupt_disabled(1, cpu);
}
void test(uint8_t arr[257])
{
	for (int i = 0; i < 257; i++)
	{
		arr[i] = i;
	}
}
/**
 * Executes actual code
 */
CPU run(CPU cpu, std::string window_name)
{

	sf::RenderWindow window(sf::VideoMode(800, 600), window_name);

	window.setFramerateLimit(144);
	sf::Texture texture;
	texture.create(256, 240);
	float scaleX = window.getSize().x / (float)(texture.getSize().x);
	float scaleY = window.getSize().y / (float)(texture.getSize().y);
	sf::Sprite sprite(texture);
	sprite.setScale(scaleX, scaleY);
	// uint8_t arr[257];
	// uint8_t data[]

	while (cpu.bus.get_PC() < PC_END && window.isOpen())
	{
		for (auto event = sf::Event{}; window.pollEvent(event);) // checks if window is closed or event going
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				program_success(cpu);
				cpu.error_code = EXIT_SUCCESS;
				return cpu;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			cpu.bus.write_controller1(Controller::UP, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::UP, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			cpu.bus.write_controller1(Controller::DOWN, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::DOWN, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			cpu.bus.write_controller1(Controller::LEFT, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::LEFT, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			cpu.bus.write_controller1(Controller::RIGHT, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::RIGHT, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			cpu.bus.write_controller1(Controller::A, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::A, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			cpu.bus.write_controller1(Controller::B, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::B, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			cpu.bus.write_controller1(Controller::START, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::START, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			cpu.bus.write_controller1(Controller::SELECT, 1);
		}
		else
		{
			cpu.bus.write_controller1(Controller::SELECT, 0);
		}

		cpu.bus.write_8bit(0xfe, ((uint8_t)rand() % 16 + 1));

		cpu.bus.tick();

		// NMI interrupts
		if (cpu.bus.NMI_interrupt())
		{
			HandleNMIInterrupts(cpu);
		}
		current_instruction = cpu.bus.fetch_next();

		if (InstructionValid(current_instruction))
		{
			Instruction a = GetInstruction(current_instruction);
			a.i(a.addressmode, cpu);
			std::vector rgb_data_vector = cpu.bus.render_texture({NES_RES_L, NES_RES_W});

			uint8_t rgb_data[NES_RES_A * 4];
			std::copy(rgb_data_vector.begin(), rgb_data_vector.end(), rgb_data);
			// nes_cpu.
			// for (int i = 0; i < nes_cpu.size(); i++)
			// {
			// 	rgb_da
			// }
			// cpu.bus.render_texture(rgb_data);
			texture.update(rgb_data);
			// printf("pc: 0x%x current instrcution 0x%x \n", cpu.bus.get_PC(), current_instruction);
			// cpu.bus.render(texture, 0, 0);
			// cpu.bus.render()
			window.clear(); // Change this to the desired color
			window.draw(sprite);
			window.display();
		}
		else
		{
			printf("instruction opcode 0x%x is unrecongnized \n", current_instruction);
			program_failure("Unrecognized instruction encountered", cpu, 1);
			cpu.error_code = EXIT_FAILURE;
			return cpu;
		}
	}
	return cpu;
}
