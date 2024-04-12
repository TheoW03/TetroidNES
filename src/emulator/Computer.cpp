#include <iostream>
#include "BitOperations.h"
#include "Memory.h"
#include "AddressMode.h"
#include "Instructions.h"
#include "StatusRegister.h"
#include <cstdio>
#include <bitset>
// #include <pthread.h>
#include "Bus.h"
#include <thread>
#include "LoadRom.h"
// #include <unistd.h>

using namespace std;

uint8_t current_instruction = 0;
uint8_t param = 0;
// struct CPUProcessor
// {
// 	uint8_t X_Reg;
// 	uint8_t Y_Reg;
// 	uint8_t A_Reg;
// 	uint16_t stack_pointer;
// 	uint16_t PC;
// 	uint8_t status;
// 	Bus bus;
// };
void run(CPUProcessor cpu_Processor);
void Init(string file_name)
{
	vector<uint8_t> test = {0xa9, 0x0, 0xf0, 0x81, 0x00};
	// Rom rom;
	// rom.PRG = test;
	// rom.CHR = test;
	// rom.mapper = 0;
	// rom.mirror = MirrorType::FOUR_SCREEN;
	vector<uint8_t> v = load_rom(file_name);
	// Bus bus(rom);
	Bus bus(modify_for_NESfile(v));
	bus.write_16bit(0xFFFC, 0x8000);
	CPUProcessor cpu_Processor;
	cpu_Processor.PC = bus.read_16bit(0xFFFC);
	cpu_Processor.A_Reg = 0;
	cpu_Processor.status = 0;
	cpu_Processor.X_Reg = 0;
	cpu_Processor.Y_Reg = 0;
	cpu_Processor.stack_pointer = 0xfd;
	cpu_Processor.bus = bus;
	run(cpu_Processor);
}
void run(CPUProcessor cpu_Processor)
{
	while (cpu_Processor.PC < 0xFFFF)
	{
		cpu_Processor.bus.print_clock();

		// cout << "a" << endl;
		if (check_brk(cpu_Processor) != 0)
		{
			continue;
		}
		current_instruction = cpu_Processor.bus.read_8bit(cpu_Processor.PC);
		cpu_Processor.PC++;
		if (current_instruction == 0xEA)
			continue;
		// _sleep(100000);
		// cout << "========" << endl;
		printf("current instruction: %x \n", current_instruction);
		// printf("A_Reg: %x \n", cpu_Processor.A_Reg);
		// printf("X_Reg: %d \n", cpu_Processor.X_Reg);
		// printf("Y_Reg: %d \n", cpu_Processor.Y_Reg);
		// printf("PC: 0x%X \n", cpu_Processor.PC);
		// printf("sp: 0x%X \n", cpu_Processor.stack_pointer);
		// printf("$78: %X \n", read_8bit(0x78));
		// printf("$79: %X \n", read_8bit(0x79));
		// printf("$7a: %X \n", read_8bit(0x7a));
		// printf("$7b: %X \n", read_8bit(0x3f0));

		// bitset<7> z(cpu_Processor.status);
		// cout << "status: 0b" << z << endl;
		// cout << "========" << endl;

		// DEBUG STUFF
		//  cout << "=======" << endl;
		// printf("instruction: %x \n", current_instruction);
		// printf("CPU PC: %x \n", cpu_Processor.PC);
		// cout << "=======" << endl;
		// END
		if (current_instruction == 0xA9 || current_instruction == 0xA5 || current_instruction == 0xB5	 //
			|| current_instruction == 0xAD || current_instruction == 0xBD || current_instruction == 0xB9 //
			|| current_instruction == 0xA1 || current_instruction == 0xB1)
		{

			LDA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xA2 || current_instruction == 0xA6 || current_instruction == 0xB6 // a
				 || current_instruction == 0xAE || current_instruction == 0xBE)
		{
			LDX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xA0 || current_instruction == 0xA4	   //
				 || current_instruction == 0xB4 || current_instruction == 0xAC // meow
				 || current_instruction == 0xBC)							   //
		{
			LDY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x69 || current_instruction == 0x65 || current_instruction == 0x75	   // meow
				 || current_instruction == 0x6D || current_instruction == 0x7D								   // meow //meow
				 || current_instruction == 0x79 || current_instruction == 0x71 || current_instruction == 0x61) // meow
		{
			ADC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xE9 || current_instruction == 0xE5 || current_instruction == 0xF5	   // meow
				 || current_instruction == 0xED || current_instruction == 0xFD								   // mrpr
				 || current_instruction == 0xF9 || current_instruction == 0xF1 || current_instruction == 0xE1) // meow
		{
			SBC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x29 || current_instruction == 0x25 || current_instruction == 0x35 // meow
				 || current_instruction == 0x2D || current_instruction == 0x3D							   // meow //meow
				 || current_instruction == 0x39 || current_instruction == 0x21 || current_instruction == 0x31)
		{
			AND(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x09 || current_instruction == 0x05 || current_instruction == 0x15 // meow
				 || current_instruction == 0x0D || current_instruction == 0x1D							   // meow //meow
				 || current_instruction == 0x19 || current_instruction == 0x01 || current_instruction == 0x11)
		{
			ORA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x2A || current_instruction == 0x26 || current_instruction == 0x36 || current_instruction == 0x2E || current_instruction == 0x3E)
		{
			ROL(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x6A || current_instruction == 0x66 || current_instruction == 0x76 || current_instruction == 0x6E || current_instruction == 0x7E)
		{
			ROR(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x86 || current_instruction == 0x96 || current_instruction == 0x8E)
		{
			STX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x84 || current_instruction == 0x94 || current_instruction == 0x8C)
		{
			STY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x85 || current_instruction == 0x95 || current_instruction == 0x8D // meow
				 || current_instruction == 0x9D || current_instruction == 0x99							   // meow //meow
				 || current_instruction == 0x81 || current_instruction == 0x91)
		{
			STA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x4A || current_instruction == 0x46 || current_instruction == 0x56 || current_instruction == 0x4E || current_instruction == 0x5E)
		{
			LSR(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x0A || current_instruction == 0x06 || current_instruction == 0x16 || current_instruction == 0x0E || current_instruction == 0x1E)
		{
			ASL(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x49 || current_instruction == 0x45 || current_instruction == 0x55 // meow
				 || current_instruction == 0x4D || current_instruction == 0x5D							   // mrpr
				 || current_instruction == 0x59 || current_instruction == 0x41 || current_instruction == 0x51)
		{
			EOR(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xE8)
		{
			INX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xC8)
		{
			INY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xCA)
		{
			DEX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xC6 || current_instruction == 0xD6 || current_instruction == 0xCE || current_instruction == 0xDE)
		{
			DEC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xE6 || current_instruction == 0xF6 || current_instruction == 0xEE || current_instruction == 0xFE)
		{
			INC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x88)
		{
			DEY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x18)
		{
			CLC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x38)
		{
			SEC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xD8)
		{
			CLD(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xF8)
		{
			SED(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x78)
		{
			SEI(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x58)
		{
			CLI(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xB8)
		{
			CLV(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x40)
		{
			RTI(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x4C || current_instruction == 0x6C)
		{
			JMP(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xF0)
		{
			BEQ(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xD0)
		{
			BNE(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xC9 || current_instruction == 0xC5 || current_instruction == 0xD5	   // meow
				 || current_instruction == 0xCD || current_instruction == 0xDD								   // mrpr
				 || current_instruction == 0xD9 || current_instruction == 0xC1 || current_instruction == 0xD1) // meow
		{
			CMP(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xC0 // meow
				 || current_instruction == 0xC4 || current_instruction == 0xCC)
		{
			CPY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xE0 // meow
				 || current_instruction == 0xE4 || current_instruction == 0xEC)
		{
			CPX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x24 || current_instruction == 0x2C)
		{
			BIT(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x90)
		{
			BCC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xB0)
		{
			BCS(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x10)
		{
			BPL(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x30)
		{
			BMI(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x50)
		{
			BVC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x70)
		{
			BVS(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x20)
		{
			JSR(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x60)
		{
			RTS(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x28)
		{
			PLP(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x08)
		{
			PHP(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x68)
		{
			PLA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x48)
		{
			PHA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xAA)
		{
			TAX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x8A)
		{
			TXA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x9A)
		{
			TXS(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xBA)
		{
			TSX(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x98)
		{
			TYA(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xA8)
		{
			TAY(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xEA)
		{
			// NOP
		}
		else if (current_instruction == 0x00)
		{
			if (check_Interrupt_disabled(cpu_Processor) != 0)
				continue;
			set_brk(cpu_Processor, 1);
			cpu_Processor.bus.write_8bit(cpu_Processor.stack_pointer, cpu_Processor.status);
			cpu_Processor.PC++;
			cpu_Processor.stack_pointer -= 2;
			cpu_Processor.bus.write_16bit(cpu_Processor.stack_pointer, cpu_Processor.PC);
			printf("A_Reg: %d \n", cpu_Processor.A_Reg);
			printf("X_Reg: %d \n", cpu_Processor.X_Reg);
			printf("Y_Reg: %d \n", cpu_Processor.Y_Reg);
			printf("PC: 0x%X \n", cpu_Processor.PC);
			printf("sp: 0x%X \n", cpu_Processor.stack_pointer);
			bitset<7> y(cpu_Processor.status);
			cout << "status: 0b" << y << endl;
			return;
		}
		else
		{
			cout << "unrecognized instruction" << endl;
			printf("current instruction 0x%x is unrecongnized \n", current_instruction);
			printf("A_Reg: %d \n", cpu_Processor.A_Reg);
			printf("X_Reg: %d \n", cpu_Processor.X_Reg);
			printf("Y_Reg: %d \n", cpu_Processor.Y_Reg);
			printf("PC: 0x%X \n", cpu_Processor.PC);
			printf("sp: 0x%X \n", cpu_Processor.stack_pointer);
			bitset<7> y(cpu_Processor.status);
			cout << "status: 0b" << y << endl;
			exit(EXIT_FAILURE);
		}
		// this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
