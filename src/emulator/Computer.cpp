#include <iostream>
#include "BitOperations.h"
#include "Memory.h"
#include "Instructions.h"
#include <cstdio>

using namespace std;

uint8_t X_REG =0;
uint8_t Y_REG = 0;
uint8_t A_Reg = 0;
uint16_t PC = 0;
uint8_t status = 0;

uint8_t current_instruction = 0;
uint8_t param = 0;


void run(){
	while(1){
		current_instruction = read_8bit(PC);
		
		PC++;
		switch(current_instruction){
			case 0xA9:
			{
				param = read_8bit(PC);
				LDA(param, A_Reg);
				PC++;
			}
			case 0x00:
			{
				printf("A_Reg: %d \n", A_Reg);
				printf("X_Reg: %d \n", X_REG);
				printf("Y_Reg: %d \n", Y_REG);
				printf("PC: %d \n", PC);
				printf("status: %d \n", status);
				return;
			}
				
		}

	}
}
