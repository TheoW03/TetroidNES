#include <iostream>
#include "BitOperations.h"
#include "Memory.h"
#include "AddressMode.h"
#include "Instructions.h"
#include "StatusRegister.h"
#include <cstdio>
#include <bitset>

using namespace std;

uint8_t current_instruction = 0;
uint8_t param = 0;

void run()
{
	CPUProcessor cpu_Processor;
	cpu_Processor.PC = read_16bit(0xFFFC);
	cpu_Processor.A_Reg = 0;
	cpu_Processor.status = 0;
	cpu_Processor.X_Reg = 0;
	cpu_Processor.Y_Reg = 0;
	cpu_Processor.stack_pointer = 0xfd;
	while (cpu_Processor.PC < 0xFFFF)
	{
		if (check_brk(cpu_Processor) != 0)
		{
			continue;
		}
		current_instruction = read_8bit(cpu_Processor.PC);
		cpu_Processor.PC++;
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

		else if (current_instruction == 0x00)
		{
			if (check_Interrupt_disabled(cpu_Processor) != 0)
				continue;
			set_brk(cpu_Processor, 1);
			write_8bit(cpu_Processor.stack_pointer, cpu_Processor.status);
			cpu_Processor.PC++;
			cpu_Processor.stack_pointer -= 2;
			write_16bit(cpu_Processor.stack_pointer, cpu_Processor.PC);
			printf("A_Reg: %d \n", cpu_Processor.A_Reg);
			printf("X_Reg: %d \n", cpu_Processor.X_Reg);
			printf("Y_Reg: %d \n", cpu_Processor.Y_Reg);
			printf("PC: 0x%X \n", cpu_Processor.PC);
			printf("sp: 0x%X \n", cpu_Processor.stack_pointer);
			bitset<7> y(cpu_Processor.status);
			cout << "status: 0b" << y << endl;
			return;
		}
	}
}
