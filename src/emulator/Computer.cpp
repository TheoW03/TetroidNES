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
	cpu_Processor.PC = 0x8000;
	cpu_Processor.A_Reg = 0;
	cpu_Processor.status = 0;
	cpu_Processor.X_Reg = 0;
	cpu_Processor.Y_Reg = 0;
	cpu_Processor.stack_pointer = 0x100;
	while (1)
	{

		current_instruction = read_8bit(cpu_Processor.PC);
		cpu_Processor.PC++;
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
		else if (current_instruction == 0x69)
		{
			ADC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0xE9)
		{
			SBC(current_instruction, cpu_Processor);
		}
		else if (current_instruction == 0x00)
		{
			if (check_Interrupt_disabled(cpu_Processor) != 0)
				continue;
			set_brk(cpu_Processor);
			printf("A_Reg: %d \n", cpu_Processor.A_Reg);
			printf("X_Reg: %d \n", cpu_Processor.X_Reg);
			printf("Y_Reg: %d \n", cpu_Processor.Y_Reg);
			printf("PC: 0x%X \n", cpu_Processor.PC);
			bitset<7> y(cpu_Processor.status);
			cout << "status: "
				 << "0b" << y << endl;
			return;
		}
	}
}
