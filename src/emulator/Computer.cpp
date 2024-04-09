#include <map>
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

typedef void (*instructionPointer) (uint8_t, CPUProcessor);

uint8_t current_instruction = 0;
uint8_t param = 0;

std::map<uint8_t, instructionPointer> instructionMap;

//std::map<uint8_t, void (uint8_t current_instruction, CPUProcessor &cpu)> instructionMap;

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
void initializeInstructionMap();

void Init(string file_name)
{
	initializeInstructionMap();
	vector<uint8_t> test = {0xa9, 0x0, 0xf0, 0x81, 0x00};
	Rom rom;
	rom.PRG = test;
	rom.CHR = test;
	rom.mapper = 0;
	rom.mirror = MirrorType::FOUR_SCREEN;
	// vector<uint8_t> v = load_rom(file_name);
	Bus bus(rom);
	// Bus bus(modify_for_NESfile(v));
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

/**
 * Loads instructions and their associated function in the emulator into a map.
 * May be read for documenting which codes are associated with each instruction.
 */
void initializeInstructionMap()
{
	instructionMap.insert(make_pair((uint8_t)0xA9, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xA5, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xB5, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xAD, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xBD, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xB9, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xA1, (instructionPointer)LDA));
	instructionMap.insert(make_pair((uint8_t)0xB1, (instructionPointer)LDA));

	instructionMap.insert(make_pair((uint8_t)0xA2, (instructionPointer)LDX));
	instructionMap.insert(make_pair((uint8_t)0xA6, (instructionPointer)LDX));
	instructionMap.insert(make_pair((uint8_t)0xB6, (instructionPointer)LDX));
	instructionMap.insert(make_pair((uint8_t)0xAE, (instructionPointer)LDX));
	instructionMap.insert(make_pair((uint8_t)0xBE, (instructionPointer)LDX));

	instructionMap.insert(make_pair((uint8_t)0xA0, (instructionPointer)LDY));
	instructionMap.insert(make_pair((uint8_t)0xA4, (instructionPointer)LDY));
	instructionMap.insert(make_pair((uint8_t)0xB4, (instructionPointer)LDY));
	instructionMap.insert(make_pair((uint8_t)0xAC, (instructionPointer)LDY));
	instructionMap.insert(make_pair((uint8_t)0xBC, (instructionPointer)LDY));

	instructionMap.insert(make_pair((uint8_t)0x69, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x65, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x75, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x6D, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x7D, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x79, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x71, (instructionPointer)ADC));
	instructionMap.insert(make_pair((uint8_t)0x61, (instructionPointer)ADC));

	instructionMap.insert(make_pair((uint8_t)0xE9, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xE5, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xF5, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xED, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xFD, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xF9, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xF1, (instructionPointer)SBC));
	instructionMap.insert(make_pair((uint8_t)0xE1, (instructionPointer)SBC));

	instructionMap.insert(make_pair((uint8_t)0x29, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x25, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x35, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x2D, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x3D, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x39, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x21, (instructionPointer)AND));
	instructionMap.insert(make_pair((uint8_t)0x31, (instructionPointer)AND));

	instructionMap.insert(make_pair((uint8_t)0x09, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x05, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x15, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x0D, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x1D, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x19, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x01, (instructionPointer)ORA));
	instructionMap.insert(make_pair((uint8_t)0x11, (instructionPointer)ORA));

	instructionMap.insert(make_pair((uint8_t)0x2A, (instructionPointer)ROL));
	instructionMap.insert(make_pair((uint8_t)0x26, (instructionPointer)ROL));
	instructionMap.insert(make_pair((uint8_t)0x36, (instructionPointer)ROL));
	instructionMap.insert(make_pair((uint8_t)0x2E, (instructionPointer)ROL));
	instructionMap.insert(make_pair((uint8_t)0x3E, (instructionPointer)ROL));

	instructionMap.insert(make_pair((uint8_t)0x6A, (instructionPointer)ROR));
	instructionMap.insert(make_pair((uint8_t)0x66, (instructionPointer)ROR));
	instructionMap.insert(make_pair((uint8_t)0x76, (instructionPointer)ROR));
	instructionMap.insert(make_pair((uint8_t)0x6E, (instructionPointer)ROR));
	instructionMap.insert(make_pair((uint8_t)0x7E, (instructionPointer)ROR));

	instructionMap.insert(make_pair((uint8_t)0x86, (instructionPointer)STX));
	instructionMap.insert(make_pair((uint8_t)0x96, (instructionPointer)STX));
	instructionMap.insert(make_pair((uint8_t)0x8E, (instructionPointer)STX));

	instructionMap.insert(make_pair((uint8_t)0x84, (instructionPointer)STY));
	instructionMap.insert(make_pair((uint8_t)0x94, (instructionPointer)STY));
	instructionMap.insert(make_pair((uint8_t)0x8C, (instructionPointer)STY));

	instructionMap.insert(make_pair((uint8_t)0x85, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x95, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x8D, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x9D, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x99, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x81, (instructionPointer)STA));
	instructionMap.insert(make_pair((uint8_t)0x91, (instructionPointer)STA));

	instructionMap.insert(make_pair((uint8_t)0x4A, (instructionPointer)LSR));
	instructionMap.insert(make_pair((uint8_t)0x46, (instructionPointer)LSR));
	instructionMap.insert(make_pair((uint8_t)0x56, (instructionPointer)LSR));
	instructionMap.insert(make_pair((uint8_t)0x4E, (instructionPointer)LSR));
	instructionMap.insert(make_pair((uint8_t)0x5E, (instructionPointer)LSR));

	instructionMap.insert(make_pair((uint8_t)0x0A, (instructionPointer)ASL));
	instructionMap.insert(make_pair((uint8_t)0x06, (instructionPointer)ASL));
	instructionMap.insert(make_pair((uint8_t)0x16, (instructionPointer)ASL));
	instructionMap.insert(make_pair((uint8_t)0x0E, (instructionPointer)ASL));
	instructionMap.insert(make_pair((uint8_t)0x1E, (instructionPointer)ASL));

	instructionMap.insert(make_pair((uint8_t)0x49, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x45, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x55, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x4D, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x5D, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x59, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x41, (instructionPointer)EOR));
	instructionMap.insert(make_pair((uint8_t)0x51, (instructionPointer)EOR));

	instructionMap.insert(make_pair((uint8_t)0xE8, (instructionPointer)INX));

	instructionMap.insert(make_pair((uint8_t)0xC8, (instructionPointer)INY));

	instructionMap.insert(make_pair((uint8_t)0xCA, (instructionPointer)DEX));

	instructionMap.insert(make_pair((uint8_t)0xC6, (instructionPointer)DEC));
	instructionMap.insert(make_pair((uint8_t)0xD6, (instructionPointer)DEC));
	instructionMap.insert(make_pair((uint8_t)0xCE, (instructionPointer)DEC));
	instructionMap.insert(make_pair((uint8_t)0xDE, (instructionPointer)DEC));

	instructionMap.insert(make_pair((uint8_t)0xE6, (instructionPointer)INC));
	instructionMap.insert(make_pair((uint8_t)0xF6, (instructionPointer)INC));
	instructionMap.insert(make_pair((uint8_t)0xEE, (instructionPointer)INC));
	instructionMap.insert(make_pair((uint8_t)0xFE, (instructionPointer)INC));

	instructionMap.insert(make_pair((uint8_t)0x88, (instructionPointer)DEY));
	
	instructionMap.insert(make_pair((uint8_t)0x18, (instructionPointer)CLC));

	instructionMap.insert(make_pair((uint8_t)0x38, (instructionPointer)SEC));

	instructionMap.insert(make_pair((uint8_t)0xD8, (instructionPointer)CLD));

	instructionMap.insert(make_pair((uint8_t)0xF8, (instructionPointer)SED));

	instructionMap.insert(make_pair((uint8_t)0x78, (instructionPointer)SEI));

	instructionMap.insert(make_pair((uint8_t)0x58, (instructionPointer)CLI));

	instructionMap.insert(make_pair((uint8_t)0xB8, (instructionPointer)CLV));

	instructionMap.insert(make_pair((uint8_t)0x40, (instructionPointer)RTI));

	instructionMap.insert(make_pair((uint8_t)0x4C, (instructionPointer)JMP));
	instructionMap.insert(make_pair((uint8_t)0x6C, (instructionPointer)JMP));

	instructionMap.insert(make_pair((uint8_t)0xF0, (instructionPointer)BEQ));

	instructionMap.insert(make_pair((uint8_t)0xD0, (instructionPointer)BNE));

	instructionMap.insert(make_pair((uint8_t)0xC9, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xC5, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xD5, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xCD, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xDD, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xD9, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xC1, (instructionPointer)CMP));
	instructionMap.insert(make_pair((uint8_t)0xD1, (instructionPointer)CMP));

	instructionMap.insert(make_pair((uint8_t)0xC0, (instructionPointer)CPY));
	instructionMap.insert(make_pair((uint8_t)0xC4, (instructionPointer)CPY));
	instructionMap.insert(make_pair((uint8_t)0xCC, (instructionPointer)CPY));

	instructionMap.insert(make_pair((uint8_t)0xE0, (instructionPointer)CPX));
	instructionMap.insert(make_pair((uint8_t)0xE4, (instructionPointer)CPX));
	instructionMap.insert(make_pair((uint8_t)0xEC, (instructionPointer)CPX));

	instructionMap.insert(make_pair((uint8_t)0x24, (instructionPointer)BIT));
	instructionMap.insert(make_pair((uint8_t)0x2C, (instructionPointer)BIT));

	instructionMap.insert(make_pair((uint8_t)0x90, (instructionPointer)BCC));
	
	instructionMap.insert(make_pair((uint8_t)0xB0, (instructionPointer)BCS));

	instructionMap.insert(make_pair((uint8_t)0x10, (instructionPointer)BPL));

	instructionMap.insert(make_pair((uint8_t)0x30, (instructionPointer)BMI));

	instructionMap.insert(make_pair((uint8_t)0x50, (instructionPointer)BVC));

	instructionMap.insert(make_pair((uint8_t)0x70, (instructionPointer)BVS));

	instructionMap.insert(make_pair((uint8_t)0x20, (instructionPointer)JSR));

	instructionMap.insert(make_pair((uint8_t)0x60, (instructionPointer)RTS));

	instructionMap.insert(make_pair((uint8_t)0x28, (instructionPointer)PLP));

	instructionMap.insert(make_pair((uint8_t)0x08, (instructionPointer)PHP));

	instructionMap.insert(make_pair((uint8_t)0x68, (instructionPointer)PLA));

	instructionMap.insert(make_pair((uint8_t)0x48, (instructionPointer)PHA));

	instructionMap.insert(make_pair((uint8_t)0xAA, (instructionPointer)TAX));

	instructionMap.insert(make_pair((uint8_t)0x8A, (instructionPointer)TXA));

	instructionMap.insert(make_pair((uint8_t)0xBA, (instructionPointer)TXS));

	instructionMap.insert(make_pair((uint8_t)0xBA, (instructionPointer)TSX));

	instructionMap.insert(make_pair((uint8_t)0x98, (instructionPointer)TYA));

	instructionMap.insert(make_pair((uint8_t)0xA8, (instructionPointer)TAY));
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
		
		//Equivalent to, in English, if instructionMap contains current_instruction.
		if (instructionMap.find(current_instruction) == instructionMap.end()) 
		{
			if (current_instruction == 0xEA)
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
		}
		else
		{ 
			instructionMap.at(current_instruction) (current_instruction, cpu_Processor);
		}

		
		// this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
